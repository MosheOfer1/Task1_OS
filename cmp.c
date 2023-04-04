#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int verbose = 0;
    int ignore_case = 0;
    int num_of_args = argc;
    int args_indexes[5] = {0, 1, 1, 1, 1};

    // Parse command line arguments
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0)
        {
            verbose = 1;
            num_of_args--;
            args_indexes[i]--;
        }
        else if (strcmp(argv[i], "-i") == 0)
        {
            ignore_case = 1;
            num_of_args--;
            args_indexes[i]--;
        }
    }

    if (num_of_args != 3)
    {
        printf("Usage: %s <file1> <file2> [-v] [-i]\n", argv[0]);
        return 1;
    }

    int file1_index = 0, file2_index = 0;
    for (int i = 0; i < argc; i++)
    {
        if (args_indexes[i] == 1)
        {
            if (file1_index == 0)
                file1_index = i;
            else
                file2_index = i;
        }
    }

    char *file1 = argv[file1_index];
    char *file2 = argv[file2_index];

    FILE *fp1, *fp2;
    int c1, c2;
    int are_same = 1;

    fp1 = fopen(file1, "r");
    if (fp1 == NULL)
    {
        if (verbose)
            printf("general failure\n");
        return 1;
    }

    fp2 = fopen(file2, "r");
    if (fp2 == NULL)
    {
        fclose(fp1);
        if (verbose)
            printf("general failure\n");
        return 1;
    }

    while (1)
    {
        c1 = fgetc(fp1);
        c2 = fgetc(fp2);

        if (ignore_case)
        {
            c1 = tolower(c1);
            c2 = tolower(c2);
        }

        if (c1 != c2)
        {
            are_same = 0;
            break;
        }

        if (c1 == EOF || c2 == EOF)
        {
            break;
        }
    }

    fclose(fp1);
    fclose(fp2);

    if (are_same)
    {
        if (verbose)
        {
            printf("equal\n");
        }
        return 0;
    }
    else
    {
        if (verbose)
        {
            printf("distinct\n");
        }
        return 1;
    }
}
