#include "minishell.h"

int	g_exit_status = 0;

static void just_execute_it_man(t_shell *shell)
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
	if (ft_strncmp(shell->commands->av[0], "cd", 2) == 0)
		change_directory(shell->commands->av[1]);
	else if (ft_strncmp(shell->commands->av[0], "pwd", 3) == 0)
		printf("%s\n", getcwd(NULL, 0));
	else if (ft_strncmp(shell->commands->av[0], "exit", 4) == 0) //Estado de salida
		exit(0);
	else if (ft_strncmp(shell->commands->av[0], "env", 3) == 0)
		ft_env();
	else if (ft_strncmp(shell->commands->av[0], "echo", 4) == 0)
		ft_echo(shell->commands);
	//else if (ft_strncmp(shell->commands->av[0], "export", 3) == 0)  //Se repiten las variables y no hay gestion de errores
		//manage_env(NULL, 1, args);
	//else if (ft_strncmp(shell->commands->av[0], "unset", 3) == 0)
		//manage_env(NULL, 2, args);
}

static void	evaluate_struct(t_shell *shell)
{
	int	i;

	i = 0;
	if (!shell || !shell->commands)
		return ;

	if (!shell->commands->next)
	{
		if(shell->commands->is_builtin)
			execute_builtin(shell);
		else
			just_execute_it_man(shell);
	}
	// else
	// 	execute_pipeline(shell, shell->commands);
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

	manage_env(envp, 0, NULL);
	init_shell(&shell);
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
			free(input);
		}
		else
			free(input);
		free(cwd);
		cleanup_shell(&shell);
	}
	return (0);
}