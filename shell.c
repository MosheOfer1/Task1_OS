#define _XOPEN_SOURCE 700
#define DEBUG 0
#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

pid_t pids[MAX_ARGS];
int num_cmds;

void sigint_handler(int sig)
{
	if (DEBUG)
		printf("Received SIGINT has %d cmds\n",num_cmds);
	for (int i = 0; i < num_cmds; i++)
	{
		if (sig == SIGINT && pids[i] != getpid())
		{
			if (DEBUG)
				printf("Received SIGINT signal.%d killing %d\n", getpid(), pids[i]);
			kill(pids[i], SIGINT);
		}
	}
	printf("\n");
}

void close_pipes(int (*pipes)[2], int num_pipes)
{
	// Close all pipes
	for (int p = 0; p < num_pipes; p++)
	{
		int c1 = close(pipes[p][0]);
		int c2 = close(pipes[p][1]);
		if (DEBUG)
			fprintf(stderr, "%d code when closed [%d] [0] by %d\n", c1, p, getpid());
		if (DEBUG)
			fprintf(stderr, "%d code when closed [%d] [1] by %d\n", c2, p, getpid());
	}
}

int main()
{
	struct sigaction sa = {
		.sa_handler = sigint_handler,
		.sa_flags = 0,
		.sa_mask = {{0}}};
	int i;
	char cmd[MAX_CMD_LEN];
	int pipes[MAX_ARGS][2];
	char **cmds[MAX_CMD_LEN];
	char *args[MAX_ARGS];
	char *token;
	int output_fd = STDOUT_FILENO; // default output is stdout

	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
	while (1)
	{
		output_fd = STDOUT_FILENO;
		num_cmds = 0;
		strcpy(cmd,"");
		while (strcmp(cmd,"") == 0)
		{
			printf("hello: ");
			fgets(cmd, 1024, stdin);
			cmd[strlen(cmd) - 1] = '\0'; // replace \n with \0
		}
		
		//printf("got: %s\n",cmd);
		i = 0;
		token = strtok(cmd, " ");
		while (token != NULL)
		{
			// pipe
			if (strcmp(token, "|") == 0)
			{
				args[i] = NULL;
				cmds[num_cmds] = (char **)malloc(sizeof(char *) * MAX_ARGS);
				for (int x = 0; x < i; x++)
				{
					cmds[num_cmds][x] = (char *)malloc(sizeof(char) * (i + 1));
					strcpy(cmds[num_cmds][x], args[x]);
				}
				num_cmds++;
				i = 0;
				token = strtok(NULL, " ");
				continue;
			}
			// redirect
			else if ((strcmp(token, ">") == 0 || strcmp(token, ">>") == 0) && i > 0)
			{
				args[i] = NULL;
				cmds[num_cmds] = (char **)malloc(sizeof(char *) * MAX_ARGS);
				for (int x = 0; x < i; x++)
				{
					cmds[num_cmds][x] = (char *)malloc(sizeof(char) * (i + 1));
					strcpy(cmds[num_cmds][x], args[x]);
				}
				char *sign = token;
				token = strtok(NULL, " ");
				// Redirect output to file
				if (strcmp(sign, ">") == 0)
				{
					output_fd = open(token, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				}
				// ">>" appends
				else
				{
					output_fd = open(token, O_WRONLY | O_CREAT | O_APPEND, 0644);
				}

				if (output_fd == -1)
				{
					if(DEBUG) perror("open");
					//exit(EXIT_FAILURE);
				}
				token = strtok(NULL, " ");
				continue;
			}

			args[i] = token;
			token = strtok(NULL, " ");
			i++;
		}
		args[i] = NULL;
		cmds[num_cmds] = (char **)malloc(sizeof(char *) * MAX_ARGS);
		for (int x = 0; x < i; x++)
		{
			cmds[num_cmds][x] = (char *)malloc(sizeof(char) * (i + 1));
			strcpy(cmds[num_cmds][x], args[x]);
		}
		num_cmds++;

		if (strcmp(cmds[0][0], "exit") == 0)
		{
			return 0;
		}

		if (DEBUG)
			printf("num of commends: %d\n", num_cmds);

		// Execute the commands using pipes and forks
		for (i = 0; i < num_cmds - 1; i++)
		{
			pipe(pipes[i]);
		}

		for (i = 0; i < num_cmds; i++)
		{
			pids[i] = fork();
			if (pids[i] == 0)
			{
				// Child process
				if (i > 0)
				{
					// Not the first
					// Redirect input from the pipe of the previous command
					if (DEBUG)
						printf("input: prev[0] = %d, from = %s\n", pipes[i - 1][0], cmds[i][0]);
					dup2(pipes[i - 1][0], STDIN_FILENO);
				}

				if (i < num_cmds - 1)
				{
					// Not the last
					// Redirect output to the pipe of the next command
					if (DEBUG)
						printf("output: p[1] = %d, from = %s\n", pipes[i][1], cmds[i][0]);
					dup2(pipes[i][1], STDOUT_FILENO);
				}
				// The last one
				else
				{
					dup2(output_fd, STDOUT_FILENO);
				}

				// Child close pipes
				close_pipes(pipes, num_cmds - 1);

				//  Execute the current command
				if (execvp(cmds[i][0], cmds[i]) < 0)
				{
					if (DEBUG)
						perror("execvp");
				}
				close_pipes(pipes, num_cmds - 1);
				exit(1);
			}
			else if (pids[i] > 0)
			{
				// Parent
				continue;
			}
			else
			{
				perror("fork");
				exit(1);
			}
		}

		// Closing all pipes
		if (DEBUG)
			printf("parent: close all pipes\n");
		close_pipes(pipes, num_cmds - 1);

		// Wait for all child processes to finish
		int status;
		for (int p1 = 0; p1 < num_cmds; p1++)
		{
			if (DEBUG)
				printf("parent: wait to %d\n", pids[p1]);
			waitpid(pids[p1], &status, 0);
		}
		if (DEBUG)
			printf("All children died\n");

		// Free the allocated bytes of cmds
		for (int x = 0; x < num_cmds; x++)
		{
			int y = 0;
			while (cmds[x][y])
			{
				cmds[x][y] = NULL;
				free(cmds[x][y]);
			}
			free(cmds[x]);
		}
	}
}
