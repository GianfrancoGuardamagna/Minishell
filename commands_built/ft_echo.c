#include "../minishell.h"

static void	there_are_params(t_cmd *command)
{
	int	i;
	int	flag;
	int	j;

	i = 1;
	j = 2;
	flag = 1;
	while (command->av[i][j])
	{
		if (command->av[i][j] != 'n')
			flag = 0;
		j++;
	}
	if (flag)
		i += 1;
	while (command->av[i])
	{
		j = 0;
		while(command->av[i][j])
		{
			write(command->out_fd, &command->av[i][j], 1);
			j++;
		}
		if (command->av[i + 1])
			write(command->out_fd, " ", 1);
		i++;
	}
	if (!flag)
		write(command->out_fd, "\n", 1);
}

void	ft_echo(t_cmd *command)
{
	int	i;

	i = 1;
	if (!command->av[i])
		write(command->out_fd, "\n", 1);
	else if (command->av[i] && ft_strncmp(command->av[1], "-n", 2) == 0)
		there_are_params(command);
	else
	{
		while (command->av[i])
		{
			write(command->out_fd, command->av[i], ft_strlen(command->av[i]));
			if (command->av[i + 1])
				write(command->out_fd, " ", 1);
			i++;
		}
		write(command->out_fd, "\n", 1);
	}
}
