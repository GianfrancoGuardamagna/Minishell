#include "../minishell.h"

char    *manage_route(char  *path)
{
	int		i;
	int		j;
	//char	*clean_path;
	int		size;

	i = 0;
	j = 0;
	size = 0;
	while(path[i])
	{
		if(path[i] == '/')
			j++;
		if(j >= 3)
			size++;
		i++;
	}
	printf("%d\n", size);
	return("hola");
}