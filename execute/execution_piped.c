#include "../minishell.h"

/*static void	execute_builtin_in_pipeline(t_shell *shell, t_cmd *command)
{
	t_cmd *original_commands;

	original_commands = shell->commands;
	shell->commands = command;
	if (ft_strncmp(command->av[0], "cd", 2) == 0)
		change_directory(command->av[1]);
	else if (ft_strncmp(command->av[0], "pwd", 3) == 0)
		ft_pwd(command);
	else if (ft_strncmp(command->av[0], "exit", 4) == 0)
		exit(0);
	else if (ft_strncmp(command->av[0], "env", 3) == 0)
		ft_env(shell);
	else if (ft_strncmp(command->av[0], "echo", 4) == 0)
		ft_echo(command);
	else if (ft_strncmp(command->av[0], "export", 6) == 0)
		export_variables(shell);
	else if (ft_strncmp(command->av[0], "unset", 5) == 0)
		unset_variables(shell);
	shell->commands = original_commands;
}

static void	im_a_child(int prev_fd, t_cmd *current, int *pipe_fd, t_shell *shell)
{
	char	**path_env;
	char	*bin_path;

	fd_redirections(prev_fd, current, pipe_fd);
	if (current->is_builtin)
	{
		execute_builtin_in_pipeline(shell, current);
		exit(0);
	}
	else
	{
		path_env = get_path_values(shell->env, "PATH");
		bin_path = find_binary(current->av[0], path_env);
		check_struct(shell);
		printf("\n");
		if (execve(bin_path, current->av, shell->env) == -1)
			error_executing(2, shell->env, current->av);
	}
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
		if (current->next)
			current->out_fd = pipe_fd[1];
		pid = fork();
		if (pid == -1)
			return (perror("fork"));
		if (pid == 0)
			im_a_child(prev_fd, current, pipe_fd, shell);
		else
			current->pid = pid;
		handle_parent_process(&prev_fd, current, pipe_fd);
		current = current->next;
	}
	if (prev_fd != STDIN_FILENO)
		close(prev_fd);
	wait_for_childs(shell);
}*/
int	execute_pipeline(t_shell *shell)
{
    t_cmd	*current;
    pid_t	*pids;
    int		count;
    int		i;
    int		status;
    int		pipe_fd[2];
    int		prev_pipe_out;

    if (!shell || !shell->commands)
        return (1);

    // Contar comandos
    current = shell->commands;
    count = 0;
    while (current)
    {
        count++;
        current = current->next;
    }

    // Asignar array de PIDs
    pids = malloc(sizeof(pid_t) * count);
    if (!pids)
        return (1);

    current = shell->commands;
    i = 0;
    prev_pipe_out = -1;

    // Ejecutar cada comando en el pipeline
    while (current)
    {
        // Crear pipe si no es el último comando
        if (current->next)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                free(pids);
                return (1);
            }
        }

        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            free(pids);
            return (1);
        }

        // Proceso hijo
        if (pids[i] == 0)
        {
            // Redirigir entrada
            if (i > 0 && prev_pipe_out > 2)
                dup2(prev_pipe_out, STDIN_FILENO);
            else if (current->in_fd != STDIN_FILENO && current->in_fd > 2)
                dup2(current->in_fd, STDIN_FILENO);

            // Redirigir salida
            if (current->next)
                dup2(pipe_fd[1], STDOUT_FILENO);
            else if (current->out_fd != STDOUT_FILENO && current->out_fd > 2)
                dup2(current->out_fd, STDOUT_FILENO);

            // Cerrar file descriptors
            if (prev_pipe_out > 2)
                close(prev_pipe_out);
            if (current->next)
            {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            // Ejecutar comando
            if (current->is_builtin)
                exit(execute_builtin(shell));
            execvp(current->av[0], current->av);
            perror(current->av[0]);
            exit(127);
        }

        // Proceso padre
        if (prev_pipe_out > 2)
            close(prev_pipe_out);
        
        if (current->next)
        {
            close(pipe_fd[1]);
            prev_pipe_out = pipe_fd[0];
        }

        current = current->next;
        i++;
    }

    // Esperar a todos los procesos hijos
    for (int j = 0; j < count; j++)
        waitpid(pids[j], &status, 0);

    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        shell->exit_status = 128 + WTERMSIG(status);

    free(pids);
    return (shell->exit_status);
}
