#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER 1024
#define ARGS_BUFFER 64

void prompt_and_read(char *line);
void parse_command(char *line, char **args);
int execute_command(char **args);

int main(int argc, char *argsv[])
{
    char line[BUFFER];
    char *args[ARGS_BUFFER];
    int continue_prompt = 1;
    while (continue_prompt)
    {
        prompt_and_read(line);
        parse_command(line, args);
        continue_prompt = execute_command(args);
    }
}

void prompt_and_read(char *line)
{
    char cwd[BUFFER];
    getcwd(cwd, sizeof(cwd));
    printf("%s@%s> ", getenv("USER"), cwd);
    fflush(stdout);
    if (fgets(line, BUFFER, stdin) == NULL)
    {
        printf("\n");
        exit(0);
    }
}
void parse_command(char *line, char **args)
{
    int count = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL && count < ARGS_BUFFER - 1)
    {
        args[count++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[count] = NULL; // for the execvp command
}

int execute_command(char **args)
{
    // Empty prompt
    if (args[0] == NULL)
    {
        return 1;
    }
    // CD prompt managed by syscall to change the CWD built in command
    if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL)
        {
            chdir(getenv("HOME"));
        }
        else if (chdir(args[1]) != 0)
        {
            perror("Error al cambiar de directorio ");
        }
        return 1;
    }

    // exit case
    if (strcmp(args[0], "exit") == 0)
    {
        return 0;
    }
    pid_t pid = fork();

    if (pid == 0)
    {
        // son case

        execvp(args[0], args);
        perror("error in bash");
        exit(1);
    }
    else if (pid > 0)
    {
        wait(NULL);
    }
    else
    {
        perror("Error at creating fork process");
    }
    return 1;
}
