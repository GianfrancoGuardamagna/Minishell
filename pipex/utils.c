#include "../minishell.h"

char	*find_binary(char *command, char **paths)
{
	char	*tmp_path;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp_path, command);
		free(tmp_path);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}