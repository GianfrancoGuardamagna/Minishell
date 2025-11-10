/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguardam <gguardam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:37:33 by gguardam          #+#    #+#             */
/*   Updated: 2025/11/10 19:35:58 by gguardam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		name_len = ft_strlen(var_assignment);
		total_len = name_len + 2;
		new_var = malloc(total_len);
		if (!new_var)
			return ;
		ft_strlcpy(new_var, var_assignment, name_len + 1);
		ft_strlcat(new_var, "=", total_len);
		var_name = var_assignment;
	}
	else
	{
		name_len = equals_pos - var_assignment;
		total_len = ft_strlen(var_assignment) + 1;
		new_var = malloc(total_len);
		if (!new_var)
			return ;
		ft_strlcpy(new_var, var_assignment, total_len);
		
		var_name = malloc(name_len + 1);
		if (!var_name)
		{
			free(new_var);
			return ;
		}
		ft_strlcpy(var_name, var_assignment, name_len + 1);
	}
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
	if (equals_pos && var_name != var_assignment)
		free(var_name);
}

static void processing_export(t_shell *shell, int i, char *equals_pos, int name_len, char *var_name)
{
	while (shell->commands->av[i])
	{
		equals_pos = ft_strchr(shell->commands->av[i], '=');
		if (equals_pos)
		{
			name_len = equals_pos - shell->commands->av[i];
			var_name = malloc(name_len + 1);
			if (!var_name)
				return ;
			ft_strlcpy(var_name, shell->commands->av[i], name_len + 1);
		}
		else
			var_name = shell->commands->av[i];
		if (is_valid_var_name(var_name))
		{
			if (equals_pos)
				add_or_modify_var(shell->env, shell->commands->av[i]);
		}
		else
			write_error_message(shell->commands->out_fd, "export", shell->commands->av[i], "not a valid identifier");
		if (equals_pos && var_name != shell->commands->av[i])
			free(var_name);
		i++;
	}
}

// TODO: Implementar mostrar variables en formato export
void	export_variables(t_shell *shell)
{
	int i;
	char *var_name;
	char *equals_pos;
	int name_len;

	if (!shell || !shell->commands || !shell->commands->av)
		return ;
	if (!shell->commands->av[1])
	{
		ft_env(shell);
		return ;
	}
	i = 1;
	
}

void	unset_variables(t_shell *shell)
{
	int i;

	if (!shell || !shell->commands || !shell->commands->av)
		return;

	i = 1;
	while (shell->commands->av[i])
	{
		if (is_valid_var_name(shell->commands->av[i]))
		{
			char *original = shell->commands->av[1];
			shell->commands->av[1] = shell->commands->av[i];
			del_var(shell);
			shell->commands->av[1] = original;
		}
		else
			write_error_message(shell->commands->out_fd, "unset", \
shell->commands->av[i], "not a valid identifier");
		i++;
	}
}
