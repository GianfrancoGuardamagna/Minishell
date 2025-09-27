#include "../minishell.h"

//This function deletes frees the variable and then took backwards the rest of vars, so it doesnt keep a blank space
static void del_var(char **env_var,  char **arg)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(arg[0]);
	while((env_var[i]) && (ft_strncmp(env_var[i], arg[0], len) != 0))
		i++;
	if (env_var[i])
    {
        free(env_var[i]);
        while (env_var[i + 1])
        {
            env_var[i] = env_var[i + 1];
            i++;
        }
        env_var[i] = NULL;
    }
}

static void add_var(char **env_var,  char **arg)
{
	
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