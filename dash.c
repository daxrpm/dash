#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

void leer_comando(char *linea)
{
    printf("dash> ");
    fflush(stdout);
    if (fgets(linea, MAX_LINE, stdin) == NULL)
    {
        printf("\n");
        exit(0);
    }
}

void parsear_comando(char *linea, char **args)
{
    int i = 0;
    char *token = strtok(linea, " \t\n");
    while (token != NULL && i < MAX_ARGS - 1)
    {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

int ejecutar_comando(char **args)
{
    if (args[0] == NULL)
        return 1;

    if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL)
        {
            fprintf(stderr, "dash: falta argumento para cd\n");
        }
        else if (chdir(args[1]) != 0)
        {
            perror("dash");
        }
        return 1;
    }

    if (strcmp(args[0], "exit") == 0)
    {
        return 0;
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(args[0], args);
        perror("dash");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        wait(NULL);
    }
    else
    {
        perror("fork");
    }
    return 1;
}

int main()
{
    char linea[MAX_LINE];
    char *args[MAX_ARGS];
    int continuar = 1;

    while (continuar)
    {
        leer_comando(linea);
        parsear_comando(linea, args);
        continuar = ejecutar_comando(args);
    }
    return 0;
}
