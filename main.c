/***************************************************************************//**
  @file         main.c
  @author       OS 2017
  @date         Thursday,  9 March 2017
  @brief        (SHell)
*******************************************************************************/


//  shell.h
//
//  Created by Dr.Mohamed.Saleh on 3/10/17.
//  edited  by Ali Abdelaal ALi on 3/12/17.

#ifndef shell_h
#define shell_h

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#endif /* shell_h */

/*
  Function Declarations for builtin shell commands:
 */
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
        "cd",
        "help",
        "exit"
};

int (*builtin_func[]) (char **) = {
        &shell_cd,
        &shell_help,
        &shell_exit
};

int shell_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   @brief Bultin command: change directory.
   @param args List of args.  args[0] is "cd".  args[1] is the directory.
   @return Always returns 1, to continue executing.
 */
int shell_cd(char **args)
{

    if (args[1] == NULL) {
        fprintf(stderr, "shell: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("shell");
        }
    }
    return 1;
}

/**
   @brief Builtin command: print help.
   @param args List of args.  Not examined.
   @return Always returns 1, to continue executing.
 */
int shell_help(char **args)
{
    int i;
    // type your code here ...
    pid_t pid = fork();
    if(pid <0)
        fprintf(stderr,"fork faild\n");
    else if(pid==0)
    {
        execvp("man",args);
    }
    else
    {
        wait(NULL);
    }

    return 1;
}

/**
   @brief Builtin command: exit.
   @param args List of args.  Not examined.
   @return Always returns 0, to terminate execution.
 */
int shell_exit(char **args)
{
    return 0;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int shell_launch(char **args)
{
    // type your code here ...
    pid_t pid = fork();
    if(pid <0)
        fprintf(stderr,"fork faild\n");
    else if(pid==0)
    {
        execvp(args[0],args);
    }
    else
    {
        wait(NULL);
    }

    return 1;
}

/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int shell_execute(char **args)
{
    int i;

    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < shell_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return shell_launch(args);
}

#define shell_RL_BUFSIZE 1024
/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *shell_read_line(void)
{
    // type your code here ...
    char * line = NULL;
    size_t size = 0 ;
    getline(&line, &size, stdin);
    return line ;
}

#define shell_TOK_BUFSIZE 64
#define shell_TOK_DELIM " \t\r\n\a"
#define shell_TOK_MAX_NUM shell_TOK_BUFSIZE
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **shell_split_line(char *line)
{
    int bufsize = shell_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token, **tokens_backup;
    // type your code here ...
    memset(tokens,'\0',bufsize);
    char delimeters [] = shell_TOK_DELIM ;
    token = strtok(line, delimeters);
    while(token!=NULL)
    {
        int length = strlen(token) ;
        tokens[position] = malloc((length+1)*sizeof(char));
        strcpy(tokens[position], token);
        tokens[position][length+1] = '\n';
        // printf("token is %s\n",tokens[position]);
        position++ ;
        token = strtok(NULL, delimeters);
    }
    // for(int i = 0 ; tokens[i] != NULL; i++)
    //   printf("tok number %d is : %s\n", i, tokens[i]);
    return tokens;
}

/**
   @brief Loop getting input and executing it.
 */
void shell_loop(void)
{
    char *line;
    char **args;
    int status;

    do {
        printf("os_2017> ");
        line = shell_read_line();
        args = shell_split_line(line);
        status = shell_execute(args);

        free(line);
        free(args);
    } while (status);
}

/**
   @brief Main entry point.
   @param argc Argument count.
   @param argv Argument vector.
   @return status code
 */
int main(int argc, char **argv)
{
    // Load config files, if any.
    printf("Welcome to OS class 2017 shell program\n");

    // Run command loop.
    shell_loop();

    // Perform any shutdown/cleanup.

    return EXIT_SUCCESS;
}