#include "../minishell.h"

static void	there_are_params(char **argv)
{
	int	i;
	int	flag;
	int	j;

	i = 1;
	j = 2;
	flag = 1;
	while (argv[i][j])
	{
		if (argv[i][j] != 'n')
			flag = 0;
		j++;
	}
	if (flag)
		i += 1;
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
}

void	ft_echo(char **argv)
{
	int	i;

	i = 1;
	if (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
		there_are_params(argv);
	else if (!argv[i])
		printf("\n");
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
