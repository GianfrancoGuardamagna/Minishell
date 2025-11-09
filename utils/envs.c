#include "../minishell.h"

//This function adds or modifies a variable like bash export
static void add_or_modify_var(char **env_var, char *var_assignment)
{
	char	*equals_pos;
	char	*var_name;
	char	*new_var;
	int		name_len;
	int		var_index;
	int		env_count;
	int		total_len;

	equals_pos = ft_strchr(var_assignment, '=');
	if (!equals_pos)
	{
		// export VAR (sin valor) - bash behavior: crea variable vacía
		name_len = ft_strlen(var_assignment);
		total_len = name_len + 2; // "VAR=" + null
		new_var = malloc(total_len);
		if (!new_var)
			return;
		ft_strlcpy(new_var, var_assignment, name_len + 1);
		ft_strlcat(new_var, "=", total_len);
		var_name = var_assignment;
	}
	else
	{
		// export VAR=value
		name_len = equals_pos - var_assignment;
		total_len = ft_strlen(var_assignment) + 1;
		new_var = malloc(total_len);
		if (!new_var)
			return;
		ft_strlcpy(new_var, var_assignment, total_len);
		
		// Crear string temporal para el nombre
		var_name = malloc(name_len + 1);
		if (!var_name)
		{
			free(new_var);
			return;
		}
		ft_strlcpy(var_name, var_assignment, name_len + 1);
	}

	// Buscar si la variable ya existe
	var_index = find_variable_index(env_var, var_name, name_len);
	
	if (var_index != -1)
	{
		// Variable existe - reemplazar
		free(env_var[var_index]);
		env_var[var_index] = new_var;
	}
	else
	{
		// Variable nueva - agregar al final
		env_count = count_env_vars(env_var);
		env_var[env_count] = new_var;
		env_var[env_count + 1] = NULL;
	}

	// Liberar memoria temporal del nombre si se creó
	if (equals_pos && var_name != var_assignment)
		free(var_name);
}

// Función principal para manejar export como en bash
void	export_variables(t_shell *shell)
{
	int i;
	char *var_name;
	char *equals_pos;
	int name_len;

	if (!shell || !shell->commands || !shell->commands->av)
		return;

	// Si no hay argumentos, mostrar todas las variables (como bash)
	if (!shell->commands->av[1])
	{
		// TODO: Implementar mostrar variables en formato export
		ft_env(shell); // Por ahora usa env
		return;
	}

	i = 1;
	while (shell->commands->av[i])
	{
		equals_pos = ft_strchr(shell->commands->av[i], '=');
		
		if (equals_pos)
		{
			// export VAR=value
			name_len = equals_pos - shell->commands->av[i];
			var_name = malloc(name_len + 1);
			if (!var_name)
				return;
			ft_strlcpy(var_name, shell->commands->av[i], name_len + 1);
		}
		else
		{
			// export VAR (sin valor)
			var_name = shell->commands->av[i];
		}

		// Validar nombre de variable
		if (is_valid_var_name(var_name))
			add_or_modify_var(shell->env, shell->commands->av[i]);
		else
			write_error_message(shell->commands->out_fd, "export", shell->commands->av[i], "not a valid identifier");

		if (equals_pos && var_name != shell->commands->av[i])
			free(var_name);
		i++;
	}
}

// Función para manejar unset
void	unset_variables(t_shell *shell)
{
	int i;

	if (!shell || !shell->commands || !shell->commands->av)
		return;

	// Procesar cada argumento de unset
	i = 1;
	while (shell->commands->av[i])
	{
		if (is_valid_var_name(shell->commands->av[i]))
		{
			// Temporalmente cambiar av[1] para usar del_var
			char *original = shell->commands->av[1];
			shell->commands->av[1] = shell->commands->av[i];
			del_var(shell);
			shell->commands->av[1] = original;
		}
		else
			write_error_message(shell->commands->out_fd, "unset", shell->commands->av[i], "not a valid identifier");
		i++;
	}
}
