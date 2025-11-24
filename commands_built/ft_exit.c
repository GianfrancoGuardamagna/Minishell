#include "../minishell.h"

extern t_shell *g_shell;

static int	is_numeric(char *str)
{
    int	i;

    i = 0;
    if (str[0] == '-' || str[0] == '+')
        i = 1;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

void	manage_exit(t_shell *shell)
{
    int	exit_code;

    printf("exit\n");
    exit_code = 0;
    if (shell->commands->av[1])
    {
        if (!is_numeric(shell->commands->av[1]))
        {
            write_error_message(STDERR_FILENO, "exit", shell->commands->av[1], "numeric argument required");
            exit_code = 2;
        }
        else
            exit_code = ft_atoi(shell->commands->av[1]);
    }
    // ✅ Libera shell global antes de salir
    if (g_shell)
    {
        cleanup_shell(g_shell);
        free(g_shell);
    }
    exit(exit_code);
}