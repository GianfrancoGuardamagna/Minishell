/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguardam <gguardam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:37:33 by gguardam          #+#    #+#             */
/*   Updated: 2025/11/18 18:31:34 by gguardam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_export_arg(t_shell *shell, char *arg)
{
	char	*equals_pos;
	char	*var_name;

	equals_pos = ft_strchr(arg, '=');
	var_name = extract_var_name(arg, equals_pos);
	if (!var_name)
		return ;
	if (is_valid_var_name(var_name))
	{
		if (equals_pos)
			add_or_modify_var(shell->env, arg);
	}
	else
		write_error_message(shell->commands->out_fd, "export", arg, \
"not a valid identifier");
	if (var_name != arg)
		free(var_name);
}

void	export_variables(t_shell *shell)
{
	int	i;

	if (!shell || !shell->commands || !shell->commands->av)
		return ;
	if (!shell->commands->av[1])
	{
		ft_export_env(shell);
		return ;
	}
	i = 1;
	while (shell->commands->av[i])
	{
		process_export_arg(shell, shell->commands->av[i]);
		i++;
	}
}

void	unset_variables(t_shell *shell)
{
	int		i;
	char	*original;

	if (!shell || !shell->commands || !shell->commands->av)
		return ;
	i = 1;
	while (shell->commands->av[i])
	{
		if (is_valid_var_name(shell->commands->av[i]))
		{
			original = shell->commands->av[1];
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
