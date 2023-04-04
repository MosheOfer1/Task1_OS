#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int verbose = 0;
    int force_case = 0;
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
        else if (strcmp(argv[i], "-f") == 0)
        {
            force_case = 1;
            num_of_args--;
            args_indexes[i]--;
        }
    }

    if (num_of_args != 3)
    {
        printf("Usage: %s <source_file> <destination_file> [-f] [-v]\n", argv[0]);
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

    FILE *src_file = fopen(argv[file1_index], "r");
    if (src_file == NULL)
    {
        if (verbose)
            printf("general failure\n");
        return 1;
    }

    if (access(argv[file2_index], F_OK) == 0 && force_case == 0)
    {
        if (verbose)
            printf("target file exist\n");
        fclose(src_file);
        return 1;
    }

    FILE *dst_file = fopen(argv[file2_index], "w");
    if (dst_file == NULL)
    {
        if (verbose)
            printf("general failure\n");
        return 1;
    }

    int ch;
    while ((ch = fgetc(src_file)) != EOF)
    {
        fputc(ch, dst_file);
    }

    fclose(src_file);
    fclose(dst_file);
    if (verbose)
        printf("success\n");

    return 0;
}
