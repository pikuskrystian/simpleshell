# Simple Shell

A lightweight shell application written in C that provides a basic command-line interface for executing system commands.
### Features

   * Supports built-in and external commands
   * Handles basic user input and command execution
   * Supports simple input/output redirection
   
## Basic loop of a shell

Now, for the basic program logic: what does the shell do during its loop? Well, a simple way to handle commands is with three steps:

   
   * __Read__: Read the command from standard input.
   * __Parse__: Separate the command string into a program and arguments.
   * __Execute__: Run the parsed command.
   
## Shell Builtins
```int ss_cd(char **args);
int ss_help(char **args); 
int ss_exit(char **args);
int ss_pwd(char **args);
int ss_echo(char **args);
int ss_clear(char **args);
```
# Installation

Compile the program using gcc:

` gcc -o simpleshell main.c `

Usage

` Run the shell:`

`./simpleshell`

Type commands as you would in a normal shell.
Use exit to terminate the shell.
Example

```  $ ls -l 
 $ echo "Hello, world!"
 $ cat file.txt
 $ exit
```
# License

This project is open-source and free to use.
    Optionally runs it on startup if a systemd service file is included
