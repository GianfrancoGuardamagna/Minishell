#include "../minishell.h"

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

static int	is_builtin_cmd(char *cmd)
{
    if (ft_strcmp(cmd, "cd") == 0
        || ft_strcmp(cmd, "pwd") == 0
        || ft_strcmp(cmd, "exit") == 0
        || ft_strcmp(cmd, "env") == 0
        || ft_strcmp(cmd, "echo") == 0
        || ft_strcmp(cmd, "export") == 0
        || ft_strcmp(cmd, "unset") == 0)
        return (1);
    return (0);
}

void	just_execute_it_man(t_shell *shell)
{
    char	**path_env;
    char	*bin_path;
    int		status;
    pid_t	pid;

    status = 0;
    pid = fork();
    if (pid == -1)
        exit (1);
    else if (pid == 0)
    {
        fd_checker(shell);
        path_env = get_path_values(shell->env, "PATH");
        bin_path = find_binary(shell->commands->av[0], path_env);
        if (!bin_path)
        {
            write_error_message(STDERR_FILENO, shell->commands->av[0], "", "command not found");
            exit(127);
        }
        if (execve(bin_path, shell->commands->av, shell->env) == -1)
            error_executing(2, shell->env, shell->commands->av);
    }
    else if (pid > 0)
        status_wait(pid, status);
}

void	execute_builtin(t_shell *shell)
{
    int	result;

    if (!shell || !shell->commands || !shell->commands->av || !shell->commands->av[0])
        return;
    if (ft_strcmp(shell->commands->av[0], "cd") == 0)
    {
        result = change_directory(shell->commands->av[1]);
        shell->exit_status = result;
    }
    else if (ft_strcmp(shell->commands->av[0], "pwd") == 0)
        ft_pwd(shell->commands);
    else if (ft_strcmp(shell->commands->av[0], "exit") == 0)
    {
        if (shell->commands->av[1])
        {
            if (!is_numeric(shell->commands->av[1]))
            {
                write_error_message(STDERR_FILENO, "exit", shell->commands->av[1], "numeric argument required");
                exit(2);
            }
            exit(ft_atoi(shell->commands->av[1]));
        }
        else
            exit(0);
    }
    else if (ft_strcmp(shell->commands->av[0], "env") == 0)
        ft_env(shell);
    else if (ft_strcmp(shell->commands->av[0], "echo") == 0)
        ft_echo(shell->commands);
    else if (ft_strcmp(shell->commands->av[0], "export") == 0)
        export_variables(shell);
    else if (ft_strcmp(shell->commands->av[0], "unset") == 0)
        unset_variables(shell);
	else
		write_error_message(STDERR_FILENO, shell->commands->av[0], "", "command not found");
}

void	execute_command(t_shell *shell)
{
    if (!shell || !shell->commands || !shell->commands->av || !shell->commands->av[0])
        return;
    if (is_builtin_cmd(shell->commands->av[0]))
    {
        execute_builtin(shell);
    }
    else
    {
        just_execute_it_man(shell);
    }
}
