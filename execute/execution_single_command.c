#include "../minishell.h"

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
		if (execve(bin_path, shell->commands->av, shell->env) == -1)
			error_executing(2, shell->env, shell->commands->av);
	}
	else if (pid > 0)
		status_wait(pid, status);
}

//Falta gestionar "export" a secas y modificar los comentarios para que queden en ingles
void	execute_builtin(t_shell *shell)
{
	if (!shell || !shell->commands || !shell->commands->av || !shell->commands->av[0])
		return;
	if (ft_strncmp(shell->commands->av[0], "cd", 2) == 0)
		change_directory(shell->commands->av[1]);
	else if (ft_strncmp(shell->commands->av[0], "pwd", 3) == 0)
		ft_pwd(shell->commands);
	else if (ft_strncmp(shell->commands->av[0], "exit", 4) == 0)
		exit(0);
	else if (ft_strncmp(shell->commands->av[0], "env", 3) == 0)
		ft_env(shell);
	else if (ft_strncmp(shell->commands->av[0], "echo", 4) == 0)
		ft_echo(shell->commands);
	else if (ft_strncmp(shell->commands->av[0], "export", 6) == 0)
		export_variables(shell);
	else if (ft_strncmp(shell->commands->av[0], "unset", 5) == 0)
		unset_variables(shell);
}
