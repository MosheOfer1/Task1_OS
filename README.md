# Task1 In Operting Systems Course
This project is divided into three parts:

1. Implementing two small programs, **cmp** and **copy**, that function like regular command-line tools.
2. Implementing a coding library with two coding methods, **codecA** and **codecB**.
3. Writing a shell program named **stshell**.

## Running the Project

1. Clone the repository to your local machine:
```shell
git clone https://github.com/MosheOfer1/Task1_OS.git
```
2. Build the project:
```shell
make all
```
3. Boom, That's it! Once you have completed these steps, you should have the project up and running on your machine.

## Part 1: CMD tools

### Tool 1: `cmp`

The "cmp" program compares two files and returns 0 if they are equal, and 1 if they are not. 
The program also supports a -v flag to output "equal" or "distinct" in addition to the return value. 
The program supports an -i flag to ignore case when comparing files. The usage and output examples are as follows:

**Usage:**

```shell
./cmp <file1> <file2> -v
```

**Output:**

```shell
equal
```


### Tool 2: `copy`

The "copy" program copies a file to another location and/or name. 
The program returns 0 on success and 1 on failure. 
The program creates a new file if it does not exist but does not overwrite a file if it already exists. 
The program supports a -v flag to output "success" if the file is copied, "target file exist" if the file already exists, 
or "general failure" if there is some other problem. The program supports a -f flag to force overwriting the target file. 
The usage and output examples are as follows:

**Usage:**

```shell
./copy <file1> <file2> -v
```

**Output:**

```shell
success
```


## Part 2: Coding library

We have two coding methods:

### Method A: `codecA`

The `codecA` method converts all lowercase characters to uppercase and all uppercase characters to lowercase. All other characters remain unchanged.

### Method B: `codecB`

The `codecB` method converts all characters to the third next character, adding 3 to the ASCII value.

To create two tools named encode and decode that utilize the libraries, follow these steps:

1. Create two separate executable files named encode and decode.

2. Both the tools will take two arguments: the first argument will be the name of the codec to use and the second argument will be the message to encode/decode.

3. The output of the tool will be the encoded/decoded message.

4. The usage of the tools will be as follows:
```shell
./encode <codec> <message>
```
Here, codec is the name of the codec you want to use, and message is the text you want to encode/decode.

## Part 3: `stshell`

The `stshell` program is a shell that allows running CMD tools and supports the following features:

The "stshell" program is a shell that has several features:

* It can run CMD tools that exist on the system using fork, exec, and wait.
* It can stop running tools by pressing Ctrl+C without killing the shell itself using a signal handler.
* It supports output redirection with ">" and ">>" and piping with "|" following pipes. For example, the command:
```shell
ls -l | grep aaa | wc
```
is supported.
* It can be stopped by the "exit" command.

The implementation is provided in the file shell.c.
## Makefile

The Makefile contains rules for building the programs, libraries, and object files. The `all` target builds all libraries and the main program. The `clean` target removes all generated files. The `cmp`, `copy`, and `stshell` targets build the corresponding programs. The `libcodecA.so` and `libcodecB.so` targets build the shared libraries. The `encode` and `decode` targets build the main programs.


