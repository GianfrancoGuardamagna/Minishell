#include "minishell.h"

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
	else if (ft_strncmp(args[0], "exit", 4) == 0)
		exit(0);
	else if (ft_strncmp(args[0], "env", 3) == 0)
		ft_env();
	else if (ft_strncmp(args[0], "echo", 4) == 0)
		ft_echo(args);
	else
		printf("Command not implemented yet: %s\n", args[0]);
}

int	main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	char	*input;
	char	*cwd;
	char	*prompt;

	manage_env(envp, 0, NULL);
	while(1)
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
			//add_history(input); //TO DO
			process_command(input);
		}

		free(input);
		free(cwd);
	}
	return (0);
}