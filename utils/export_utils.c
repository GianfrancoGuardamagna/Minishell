#include "../minishell.h"

char	*create_var_without_value(char *var_assignment, int *name_len)
{
	char	*new_var;
	int		total_len;

	*name_len = ft_strlen(var_assignment);
	total_len = *name_len + 2;
	new_var = malloc(total_len);
	if (!new_var)
		return (NULL);
	ft_strlcpy(new_var, var_assignment, *name_len + 1);
	ft_strlcat(new_var, "=", total_len);
	return (new_var);
}

char	*create_var_with_value(char *var_assignment, char *equals_pos, \
char **var_name, int *name_len)
{
	char	*new_var;
	int		total_len;

	*name_len = equals_pos - var_assignment;
	total_len = ft_strlen(var_assignment) + 1;
	new_var = malloc(total_len);
	if (!new_var)
		return (NULL);
	ft_strlcpy(new_var, var_assignment, total_len);
	*var_name = malloc(*name_len + 1);
	if (!*var_name)
	{
		free(new_var);
		return (NULL);
	}
	ft_strlcpy(*var_name, var_assignment, *name_len + 1);
	return (new_var);
}

void	update_env_array(char **env_var, char *new_var, char *var_name, \
int name_len)
{
	int	var_index;
	int	env_count;

	var_index = find_variable_index(env_var, var_name, name_len);
	if (var_index != -1)
	{
		free(env_var[var_index]);
		env_var[var_index] = new_var;
	}
	else
	{
		env_count = count_env_vars(env_var);
		env_var[env_count] = new_var;
		env_var[env_count + 1] = NULL;
	}
}

void	add_or_modify_var(char **env_var, char *var_assignment)
{
	char	*equals_pos;
	char	*var_name;
	char	*new_var;
	int		name_len;

	equals_pos = ft_strchr(var_assignment, '=');
	if (!equals_pos)
	{
		new_var = create_var_without_value(var_assignment, &name_len);
		var_name = var_assignment;
	}
	else
	{
		new_var = create_var_with_value(var_assignment, equals_pos,
				&var_name, &name_len);
	}
	if (!new_var)
		return ;
	update_env_array(env_var, new_var, var_name, name_len);
	if (equals_pos && var_name != var_assignment)
		free(var_name);
}

char	*extract_var_name(char *var_assignment, char *equals_pos)
{
	char	*var_name;
	int		name_len;

	if (equals_pos)
	{
		name_len = equals_pos - var_assignment;
		var_name = malloc(name_len + 1);
		if (!var_name)
			return (NULL);
		ft_strlcpy(var_name, var_assignment, name_len + 1);
	}
	else
		var_name = var_assignment;
	return (var_name);
}
