#include "../minishell.h"

static void	there_are_params(t_cmd *command)
{
	int	i;
	int	flag;
	int	j;

	i = 1;
	j = 2;
	flag = 1;
	while (command->av[i])
	{
		if (command->av[i] != 'n')
			flag = 0;
		j++;
	}
	if (flag)
		i += 1;
	while (command->av[i])
	{
		printf("%s", command->av[i]); //while que escriba en write el fd
		if (command->av[i][i + 1])
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
}

void	ft_echo(t_cmd *command)
{
	int	i;

	i = 1;
	if (command->av[1] && ft_strncmp(command->av[1], "-n", 2) == 0)
		there_are_params(command);
	else if (!argv[i])
		write("\n");
	else
	{
		while (argv[i])
		{
			printf("%s", argv[i]);
			if (argv[i + 1])
				printf(" ");
			i++;
		}
		printf("\n");
	}
}
