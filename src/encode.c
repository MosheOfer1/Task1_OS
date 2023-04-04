#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

int main(int argc, char *argv[])
{
    // Check for correct usage
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <codecA|codecB> <message>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "codecA") != 0 && strcmp(argv[1], "codecB") != 0)
    {
        printf("Invalid codec: %s\n", argv[1]);
        return 1;
    }

    // Load the appropriate shared library based on codec argument
    void *lib_handle;
    void (*EncodeFunc)(char *, char *);
    // void (*DecodeFunc)(char *, char *);
    if (strcmp(argv[2], "codecA") == 0)
    {
        lib_handle = dlopen("./lib/libcodecA.so", RTLD_LAZY);
    }
    else
    {
        lib_handle = dlopen("./lib/libcodecB.so", RTLD_LAZY);
    }
    if (!lib_handle)
    {
        printf("Error loading shared library: %s\n", dlerror());
        return 1;
    }

    // Get function pointers to encode and decode functions
    *(void **)(&EncodeFunc) = dlsym(lib_handle, "encode");
    // *(void **)(&DecodeFunc) = dlsym(lib_handle, "decode");
    const char *dlsym_error = dlerror();
    if (dlsym_error)
    {
        fprintf(stderr, "Error getting function pointer: %s\n", dlsym_error);
        dlclose(lib_handle);
        return 1;
    }

    // Allocate memory for input and output messages
    char *input = argv[2];
    char *output = (char *)malloc(strlen(input) + 1);

    // Call the appropriate function to encode/decode the message
    (*EncodeFunc)(input, output);
    // (*DecodeFunc)(input,output);

    // Print the resulting message to the console
    printf("%s\n", output);

    // Clean up and exit
    free(output);
    dlclose(lib_handle);
    return 0;
}
