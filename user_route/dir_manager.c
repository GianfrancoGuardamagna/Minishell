#include "../minishell.h"

int	change_directory(char *path)
{
	char	*expanded_path;
	char	*oldpwd;
	char	cwd_buffer[4096];

	if (!path)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
		oldpwd = getcwd(cwd_buffer, sizeof(cwd_buffer));
		if (oldpwd)
			setenv("OLDPWD", oldpwd, 1);
		chdir(path);
		getcwd(cwd_buffer, sizeof(cwd_buffer));
		setenv("PWD", cwd_buffer, 1);
	}
	else if (ft_strncmp(path, "-", 1) == 0 && path[1] == '\0')
	{
		oldpwd = getenv("OLDPWD");
		if (!oldpwd)
		{
			printf("cd: OLDPWD not set\n");
			return (1);
		}
		printf("%s\n", oldpwd);
		setenv("OLDPWD", getenv("PWD"), 1);
		chdir(oldpwd);
		getcwd(cwd_buffer, sizeof(cwd_buffer));
		setenv("PWD", cwd_buffer, 1);
	}
	else if (path[0] == '~' && (path[1] == '\0' || path[1] == '/'))
	{
		expanded_path = ft_strjoin(getenv("HOME"), path + 1);
		if (!expanded_path)
			return (1);
		oldpwd = getcwd(cwd_buffer, sizeof(cwd_buffer));
		if (oldpwd)
			setenv("OLDPWD", oldpwd, 1);
		if (chdir(expanded_path) != 0)
		{
			perror("cd");
			free(expanded_path);
			return (1);
		}
		getcwd(cwd_buffer, sizeof(cwd_buffer));
		setenv("PWD", cwd_buffer, 1);
		free(expanded_path);
	}
	else
	{
		oldpwd = getcwd(cwd_buffer, sizeof(cwd_buffer));
		if (oldpwd)
			setenv("OLDPWD", oldpwd, 1);
		if (chdir(path) != 0)
		{
			perror("cd");
			return (1);
		}
		getcwd(cwd_buffer, sizeof(cwd_buffer));
		setenv("PWD", cwd_buffer, 1);
	}
	return (0);
}

char	*format_cwd(char *cwd)
{
	char	*processed_cwd;
	char	*result;
	size_t	len;
	char	*home;

	home = ft_strjoin("/home/", getenv("USER"));
	if (!home) // ✅ Valida si ft_strjoin falló
		return (ft_strdup("Minishell$ "));

	if (ft_strnstr(cwd, home, ft_strlen(home)))
	{
		if (ft_strlen(cwd) == ft_strlen(home)) // Compara longitudes
			processed_cwd = ft_strdup("~");
		else
			processed_cwd = ft_strjoin("~", cwd + ft_strlen(home));
	}
	else
		processed_cwd = ft_strdup(cwd);
	
	free(home); // ✅ Libera 'home' tan pronto como ya no se necesite

	if (!processed_cwd)
		return (ft_strdup("Minishell$ "));
	
	len = ft_strlen(processed_cwd) + 3;
	result = malloc(len);
	if (!result)
	{
		free(processed_cwd);
		return (ft_strdup("Minishell$ "));
	}
	ft_strlcpy(result, processed_cwd, len);
	ft_strlcat(result, "$ ", len);
	free(processed_cwd);
	return (result);
}
