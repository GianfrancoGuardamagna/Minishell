#include "minishell.h"

// Cuenta cuántas variables hay en el array
int	count_env_vars(char **env_var)
{
	int count;

	count = 0;
	while (env_var[count])
		count++;
	return (count);
}

int	find_variable_index(char **env_var, char *var_name, int name_len)
{
	int	i;

	i = 0;
	while (env_var[i])
	{
		// Verificar que coincida el nombre exacto y que después venga '='
		if (ft_strncmp(env_var[i], var_name, name_len) == 0 && 
			env_var[i][name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

// Valida si el nombre de variable es válido según las reglas de bash
int	is_valid_var_name(char *name)
{
	int i;

	if (!name || !name[0])
		return (0);
	
	// Primer caracter: letra o underscore
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	
	// Resto: letras, números o underscores
	i = 1;
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

//This function deletes frees the variable and then took backwards the rest of vars, so it doesnt keep a blank space
void del_var(t_shell *shell)
{
	int	i;
	int	var_name_len;
	char *var_to_delete;

	if (!shell || !shell->commands || !shell->commands->av[1])
		return;
	var_to_delete = shell->commands->av[1];
	var_name_len = ft_strlen(var_to_delete);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], var_to_delete, var_name_len) == 0 &&
			(shell->env[i][var_name_len] == '=' || shell->env[i][var_name_len] == '\0'))
		{
			while (shell->env[i + 1])
			{
				shell->env[i] = shell->env[i + 1];
				i++;
			}
			shell->env[i] = NULL;
			break;
		}
		i++;
	}
}