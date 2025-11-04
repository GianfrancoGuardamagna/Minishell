#include "../minishell.h"

//This function deletes frees the variable and then took backwards the rest of vars, so it doesnt keep a blank space
static void del_var(char **env_var,  char **arg)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(arg[1]);
	while((env_var[i]) && (ft_strncmp(env_var[i], arg[1], len) != 0))
		i++;
	if (env_var[i])
	{
		while (env_var[i + 1])
		{
			env_var[i] = env_var[i + 1];
			i++;
		}
		env_var[i] = NULL;
	}
}

static int	overwrite_variable(char **env_var, char	*new_var)
{
	int	i;
	int	j;
	int	new_var_name_len;

	i = 0;
	j = 0;
	while(new_var[i] != '=')
		i++;
	new_var_name_len = i;
	i = 0;
	while(env_var[i])
	{
		j = 0;
		while((env_var[i][j] == new_var[j]) && env_var[i][j] && new_var[j])
			j++;
		if(j == new_var_name_len && env_var[i][j] == '=')
			return (i);
		i++;
	}
	return (-1);
}

//This function adds a variable
static void add_var(char **env_var, char **arg)
{
	int		i;
	int		var_len;
	char	*new_var;
	int		pos_var;

	i = 0;
	if(ft_strchr(arg[1], '=') && arg[1][0] != '=')
	{
		var_len = ft_strlen(arg[1]) + 1; // Length + null terminator
		new_var = malloc(var_len); // Allocate memory
		if (!new_var)
			return;
		ft_strlcpy(new_var, arg[1], var_len); // Copy the entire "VAR=value"
		while (env_var[i])
			i++;
		pos_var = overwrite_variable(env_var, new_var);
		if(pos_var != -1)
		{
			free(env_var[pos_var]);
			env_var[pos_var] = new_var;
		}
		else
		{
			env_var[i] = new_var;
			env_var[i + 1] = NULL;
		}
	}
}

char **manage_env(char **envp, int type, char **arg)
{
	static	char **env_var;

	if(envp && type == 0 && !arg)
		env_var = envp;
	else if(!envp && type == 0 && !arg)
		return (env_var);
	else if(!envp && type == 1 && arg)
		add_var(env_var, arg);
	else if(!envp && type == 2 && arg)
		del_var(env_var, arg);
	return (NULL);
}