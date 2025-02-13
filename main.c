#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

/* Function declarations */
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_pwd(char **args);
int lsh_echo(char **args);
int lsh_clear(char **args);

char *builtin_str[] = {
    "cd", "help", "exit", "pwd", "echo", "clear"};

int (*builtin_func[])(char **) = {
    &lsh_cd, &lsh_help, &lsh_exit, &lsh_pwd, &lsh_echo, &lsh_clear};

int lsh_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

void handle_sigint(int sig)
{
    printf("\nUse 'exit' to quit.\n> ");
    fflush(stdout);
}

int lsh_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("lsh");
        }
    }
    return 1;
}

int lsh_help(char **args)
{
    printf("LSH Shell\nBuilt-in commands:\n");
    for (int i = 0; i < lsh_num_builtins(); i++)
    {
        printf("  %s\n", builtin_str[i]);
    }
    return 1;
}

int lsh_exit(char **args)
{
    return 0;
}

int lsh_pwd(char **args)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("lsh");
    }
    return 1;
}

int lsh_echo(char **args)
{
    for (int i = 1; args[i] != NULL; i++)
    {
        printf("%s ", args[i]);
    }
    printf("\n");
    return 1;
}

int lsh_clear(char **args)
{
    printf("\033[H\033[J");
    return 1;
}

int lsh_launch(char **args)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
            perror("lsh");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("lsh");
    }
    else
    {
        int status;
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int lsh_execute(char **args)
{
    if (args[0] == NULL)
        return 1;
    for (int i = 0; i < lsh_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }
    return lsh_launch(args);
}

char *lsh_read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

char **lsh_split_line(char *line)
{
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token = strtok(line, " \t\r\n\a");
    while (token)
    {
        tokens[position++] = token;
        if (position >= bufsize)
            tokens = realloc(tokens, (bufsize += 64) * sizeof(char *));
        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[position] = NULL;
    return tokens;
}

void lsh_loop(void)
{
    signal(SIGINT, handle_sigint);
    char *line;
    char **args;
    int status;
    do
    {
        printf("> ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);
        free(line);
        free(args);
    } while (status);
}

int main()
{
    lsh_loop();
    return EXIT_SUCCESS;
}
