#include "minishell.h"

static void just_execute_it_man(char **args)
{
	char	**env;
	char	**path_env;
	char	*bin_path;
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit (1);
	else if (pid == 0)
	{
		env = manage_env(NULL, 0, NULL);
		path_env = ft_split(getenv("PATH"), ':');
		bin_path = find_binary(args[0], path_env);
		if (execve(bin_path, args, env) == -1)
			error_executing(2, env, args);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		return ;
	}
}

static void shall_i_pipe_it(char **args)
{
	just_execute_it_man(args);
	//Insted of looping args, it should loop *t_cmd
	//i = 0;
	//while(args[i])
	//	i++;
	//if(i == 1)
	//	just_execute_it_man(args[0]); //SINGEL CMD WITHOUT FD MODS
	//else
	//	return ;//okay_i_need_to_pipe_it()
}

void process_command(char *input)
{
	char **args;
	
	args = ft_split(input, ' ');
	if(!args || !args[0])
		return ;
	if (ft_strncmp(args[0], "cd", 2) == 0)
		change_directory(args[1]);
	else if (ft_strncmp(args[0], "pwd", 3) == 0)
		printf("%s\n", getcwd(NULL, 0));
	else if (ft_strncmp(args[0], "exit", 4) == 0) //Estado de salida
		exit(0);
	else if (ft_strncmp(args[0], "env", 3) == 0)
		ft_env();
	else if (ft_strncmp(args[0], "export", 3) == 0)  //Se repiten las variables y no hay gestion de errores
		manage_env(NULL, 1, args);
	else if (ft_strncmp(args[0], "unset", 3) == 0)
		manage_env(NULL, 2, args);
	else if (ft_strncmp(args[0], "echo", 4) == 0)
		ft_echo(args);
	else
		shall_i_pipe_it(args); // NO SE PUEDEN UTILIZAR LOS CMDS VIM, TOUCH, NANO, ETC
		//printf("Command not implemented yet: %s\n", args[0]);
}

int	main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	char	*input;
	char	*cwd;
	char	*prompt;
	struct sigaction sa;

	manage_env(envp, 0, NULL);
	
	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
	while(1) //Se queda pegado con el CTRL C
	{
		cwd = getcwd(NULL, 0);
		prompt = format_cwd(cwd);
		input = readline(prompt);

		// If input is NULL (Ctrl-D), exit
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
			process_command(input);
		}

		free(input);
		free(cwd);
	}
	return (0);
}