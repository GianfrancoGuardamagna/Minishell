#include "minishell.h"

int g_exit_status = 0;

static void execute_builtin_in_pipeline(t_shell *shell, t_cmd *command)
{
	// Guardar el comando actual temporalmente
	t_cmd *original_commands = shell->commands;
	shell->commands = command;
	
	// Ejecutar el built-in específico
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
	
	// Restaurar el comando original
	shell->commands = original_commands;
}

static void execute_pipeline(t_shell *shell)
{
	t_cmd	*current;
	int		pipe_fd[2];
	pid_t	pid;
	int		prev_fd;
	
	current = shell->commands;
	prev_fd = STDIN_FILENO;
	
	while (current)
	{
		// Crear pipe si no es el último comando
		if (current->next)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				return;
			}
		}
		
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return;
		}
		else if (pid == 0)
		{
			// Proceso hijo
			// Configurar entrada del comando anterior
			if (prev_fd != STDIN_FILENO)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			
			// Configurar salida hacia el siguiente comando
			if (current->next)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
				close(pipe_fd[0]);
			}
			
			// Aplicar redirecciones específicas del comando
			if (current->out_fd != STDOUT_FILENO && current->out_fd > 0)
			{
				dup2(current->out_fd, STDOUT_FILENO);
				close(current->out_fd);
			}
			if (current->in_fd != STDIN_FILENO && current->in_fd > 0)
			{
				dup2(current->in_fd, STDIN_FILENO);
				close(current->in_fd);
			}
			
			// Ejecutar comando
			if (current->is_builtin)
			{
				// Para built-ins en pipelines, necesitamos manejarlos especialmente
				execute_builtin_in_pipeline(shell, current);
				exit(0);
			}
			else
			{
				char **path_env = ft_split(getenv("PATH"), ':');
				char *bin_path = find_binary(current->av[0], path_env);
				if (execve(bin_path, current->av, shell->env) == -1)
					error_executing(2, shell->env, current->av);
			}
		}
		else
		{
			// Proceso padre
			// Cerrar el extremo de escritura del pipe anterior
			if (prev_fd != STDIN_FILENO)
				close(prev_fd);
			
			// Cerrar el extremo de escritura del pipe actual
			if (current->next)
			{
				close(pipe_fd[1]);
				prev_fd = pipe_fd[0];
			}
			
			current = current->next;
		}
	}
	
	// Cerrar el último file descriptor
	if (prev_fd != STDIN_FILENO)
		close(prev_fd);
	
	// Esperar a todos los procesos hijo
	current = shell->commands;
	while (current)
	{
		int status;
		wait(&status);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
		current = current->next;
	}
}

void just_execute_it_man(t_shell *shell)
{
	char	**path_env;
	char	*bin_path;
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit (1);
	else if (pid == 0)
	{
		// Aplicar redirecciones en el proceso hijo
		if (shell->commands->out_fd != STDOUT_FILENO && shell->commands->out_fd > 0)
		{
			dup2(shell->commands->out_fd, STDOUT_FILENO);
			close(shell->commands->out_fd);
		}
		if (shell->commands->in_fd != STDIN_FILENO && shell->commands->in_fd > 0)
		{
			dup2(shell->commands->in_fd, STDIN_FILENO);
			close(shell->commands->in_fd);
		}
		
		path_env = ft_split(getenv("PATH"), ':');
		bin_path = find_binary(shell->commands->av[0], path_env);
		if (execve(bin_path, shell->commands->av, shell->env) == -1)
			error_executing(2, shell->env, shell->commands->av);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
		else
			g_exit_status = 1;
	}
}

static void execute_builtin(t_shell *shell)
{
	if (!shell || !shell->commands || !shell->commands->av || !shell->commands->av[0])
		return;
		
	if (ft_strncmp(shell->commands->av[0], "cd", 2) == 0)
		change_directory(shell->commands->av[1]);
	else if (ft_strncmp(shell->commands->av[0], "pwd", 3) == 0)
		ft_pwd(shell->commands);
	else if (ft_strncmp(shell->commands->av[0], "exit", 4) == 0) //Estado de salida
		exit(0);
	else if (ft_strncmp(shell->commands->av[0], "env", 3) == 0)
		ft_env(shell);
	else if (ft_strncmp(shell->commands->av[0], "echo", 4) == 0)
		ft_echo(shell->commands);
	else if (ft_strncmp(shell->commands->av[0], "export", 6) == 0) //Falta gestionar "export" a secas y modificar los comentarios para que queden en ingles
		export_variables(shell);
	else if (ft_strncmp(shell->commands->av[0], "unset", 5) == 0)
		unset_variables(shell);
}

static void	evaluate_struct(t_shell *shell)
{
	if (!shell || !shell->commands)
		return ;
	//check_struct(shell);
		//printf("\n");
	if (!shell->commands->next)
	{
		if(shell->commands->is_builtin)
			execute_builtin(shell);
		else
			just_execute_it_man(shell);
	}
	else if (shell->commands->next)
		execute_pipeline(shell);
}

int	main(int argc, char **argv, char **envp)
{
	(void)	argc;
	(void)	argv;
	char	*input;
	char	*cwd;
	char	*prompt;
	t_shell	shell;
	struct	sigaction sa;

	init_shell(&shell, envp);
	rl_catch_signals = 0; //Variable global definida en la libreria readline
	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
	while(1)
	{
		cwd = getcwd(NULL, 0);
		prompt = format_cwd(cwd);
		input = readline(prompt);
		if (!input)
		{
			printf("exit\n");
			free(cwd);
			free(input);
			break;
		}
		else if (*input)
		{
			add_history(input);
			shell.tokens = tokenize(input);
			if (shell.tokens)
			{
				expand_variables(&shell, shell.tokens);
				shell.commands = parse_tokens(&shell.tokens);
				
				if (shell.commands)
					evaluate_struct(&shell);
			}
			shell.exit_status = g_exit_status;
			free(input);
		}
		else
			free(input);
		free(cwd);
		cleanup_shell(&shell);
		//check_struct(&shell);
		//printf("\n");
	}
	return (0);
}
