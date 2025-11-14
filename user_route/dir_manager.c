#include "../minishell.h"

int	change_directory(char *path)
{
	if (!path)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
		chdir(path);
	}
	else if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

char	*format_cwd(char *cwd)
{
	char	*processed_cwd;
	size_t	len;
	char	*home;

	home = ft_strjoin("/home/", getenv("USER"));
	if (ft_strnstr(cwd, home, ft_strlen(home)))
	{
		if (ft_strlen(cwd) == 16)
			processed_cwd = ft_strdup("~");
		else
			processed_cwd = ft_strjoin("~", cwd + ft_strlen(home));
	}
	else
		processed_cwd = ft_strdup(cwd);
	if (!processed_cwd)
		return ("Minishell$ ");
	len = ft_strlen(processed_cwd) + 3;
	ft_strlcat(processed_cwd, "$ ", len);
	return (processed_cwd);
}
