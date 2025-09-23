#include "minishell.h"

int	main(void)
{
	char	*input;
	char *cwd;

	cwd = getcwd(NULL, 0);
	input = readline(cwd);
	while(input != NULL)
	{
		printf("%s\n", input);
		free(input);
		input = readline(cwd);
	}
	manage_route(cwd);
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}