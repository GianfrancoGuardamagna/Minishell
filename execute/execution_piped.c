#include "../minishell.h"

static int	is_builtin_cmd(char *cmd)
{
    if (!cmd)
        return (0);
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

static void	execute_builtin_in_pipeline(t_shell *shell, t_cmd *command)
{
    t_cmd *original_commands;

    original_commands = shell->commands;
    shell->commands = command;
    if (ft_strcmp(command->av[0], "cd") == 0)
        change_directory(command->av[1]);
    else if (ft_strcmp(command->av[0], "pwd") == 0)
        ft_pwd(command);
    else if (ft_strcmp(command->av[0], "exit") == 0)
        exit(0);
    else if (ft_strcmp(command->av[0], "env") == 0)
        ft_env(shell);
    else if (ft_strcmp(command->av[0], "echo") == 0)
        ft_echo(command);
    else if (ft_strcmp(command->av[0], "export") == 0)
        export_variables(shell);
    else if (ft_strcmp(command->av[0], "unset") == 0)
        unset_variables(shell);
    shell->commands = original_commands;
}

static void	im_a_child(int prev_fd, t_cmd *current, int *pipe_fd, t_shell *shell)
{
    char	**path_env;
    char	*bin_path;

    fd_redirections(prev_fd, current, pipe_fd);
    if (is_builtin_cmd(current->av[0]))
    {
        execute_builtin_in_pipeline(shell, current);
        exit(0);
    }
    path_env = get_path_values(shell->env, "PATH");
    bin_path = find_binary(current->av[0], path_env);
    if (!bin_path)
    {
        write_error_message(STDERR_FILENO, current->av[0], "", "command not found");
        exit(127);
    }
    if (execve(bin_path, current->av, shell->env) == -1)
        error_executing(2, shell->env, current->av);
}

static void	handle_parent_process(int *prev_fd, t_cmd *current, int *pipe_fd)
{
    if (*prev_fd != STDIN_FILENO)
        close(*prev_fd);
    if (current->next)
    {
        close(pipe_fd[1]);
        *prev_fd = pipe_fd[0];
    }
}

void	execute_pipeline(t_shell *shell)
{
    t_cmd	*current;
    int		pipe_fd[2];
    pid_t	pid;
    int		prev_fd;

    current = shell->commands;
    prev_fd = STDIN_FILENO;
    while (current)
    {
        if (create_pipe_if_needed(current, pipe_fd))
            return ;
        pid = fork();
        if (pid == -1)
            return (perror("fork"));
        if (pid == 0)
            im_a_child(prev_fd, current, pipe_fd, shell);
        handle_parent_process(&prev_fd, current, pipe_fd);
        current = current->next;
    }
    if (prev_fd != STDIN_FILENO)
        close(prev_fd);
    wait_for_childs(shell);
}
