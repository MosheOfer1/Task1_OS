
#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    char cmd[MAX_CMD_LEN];
    char *cmds[MAX_ARGS];
    char *args[MAX_ARGS];
    int i, j, num_cmds = 0, num_args = 0;
    int in_pipe = 0;
    int pipefd[2];
    pid_t pid;
    // fprintf(stderr,"%s\n",argv[1]);
    // Make sure there is at least one command
    if (argc < 2) {
        printf("Usage: %s cmd1 | cmd2 | ... | cmdN\n", argv[0]);
        return 1;
    }
    // Copy the argument string to cmd variable
    strncpy(cmd, argv[1], MAX_CMD_LEN);

    while (cmd[i]!=0)
    {
        fprintf(stderr,"i = %d cmd = %c\n",i,cmd[i]);
        i++;
    }
    


    // Parse the command into separate commands based on the '|' character
    for (i = 0; cmd[i]; i++) {
        fprintf(stderr,"i = %d\n",i);
        if (cmd[i] == '|') {
            // Replace '|' with '\0' to terminate the current command string
            cmd[i] = '\0';
            cmds[num_cmds] = cmd + in_pipe;
            num_cmds++;

            // Set the starting position of the next command string
            in_pipe = i + 1;
        } else if (cmd[i] == '"' || cmd[i] == '\'') {
            // Quoted string, find the end of the string
            char quote = cmd[i];
            int start = i + 1, end = start;
            while (cmd[end] && cmd[end] != quote) end++;
            if (cmd[end] == quote) {
                cmd[end] = '\0';
                args[num_args++] = cmd + start;
                i = end;
            } else {
                fprintf(stderr, "Unmatched %c\n", quote);
                return 1;
            }
        } else if (cmd[i] == ' ' || cmd[i] == '\t') {
            // Whitespace, skip it
            continue;
        } else {
            // Argument, find the end of the argument
            int start = i, end = start;
            while (cmd[end] && cmd[end] != ' ' && cmd[end] != '\t' && cmd[end] != '|' && cmd[end] != '"' && cmd[end] != '\'') end++;
            cmd[end] = '\0';
            args[num_args++] = cmd + start;
            i = end - 1;
        }
    }
    // Add the last command string to the array
    cmds[num_cmds] = cmd + in_pipe;
    num_cmds++;

    // Execute the commands using pipes and forks
    for (i = 0; i < num_cmds; i++) {
        if (i < num_cmds - 1) {
            // Create a pipe for the output of the current command
            pipe(pipefd);
        }

        pid = fork();
        if (pid == 0) {
            // Child process
            if (i > 0) {
                // Redirect input from the pipe of the previous command
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            if (i < num_cmds - 1) {
                // Redirect output to the pipe of the next command
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            // Execute the current command
            fprintf(stderr,"%s\n",cmds[i]);
            if (execvp(cmds[i], &cmds[i]) < 0) {
                perror("execvp");
                exit(1);
            }
        } else if (pid > 0) {
            // Parent process
            if (i > 0) {
                // Close the input pipe of the previous command
                close(pipefd[0]);
                close(pipefd[1]);
            }
        } else {
            perror("fork");
            exit(1);
        }
    }

    // Wait for all child processes to finish
    while (wait(NULL) > 0);

    return 0;
}
