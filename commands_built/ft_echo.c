#include "../minishell.h"

void	ft_echo(char **argv)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if(ft_strncmp(argv[i], "-n", 2) == 0)
	{
		flag = 1;
		i += 1;
	}
	while(argv[i])
	{
		printf("%s ", argv[i]);
		i++;
	}
	if(!flag)
		printf("\n");
}