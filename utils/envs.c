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

//This function adds a variable
static void add_var(char **env_var, char **arg)
{
	int i = 0;
	int var_len;
	char *new_var;

	while (env_var[i])
		i++;
	var_len = ft_strlen(arg[1]) + 1; // $NAME + 1 for '='
	if (arg[2])
		var_len += ft_strlen(arg[2]); // $VALUE
	new_var = malloc(var_len + 1); // Allocate memory
	if (!new_var)
		return;
	ft_strlcpy(new_var, arg[1], ft_strlen(arg[1]) + 1);
	ft_strlcat(new_var, "=", var_len + 1);
	if (arg[2])
		ft_strlcat(new_var, arg[2], var_len + 1);
	env_var[i] = new_var;
	env_var[i + 1] = NULL;
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