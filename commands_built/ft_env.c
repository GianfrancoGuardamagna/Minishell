/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguardam <gguardam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:35:24 by gguardam          #+#    #+#             */
/*   Updated: 2025/11/10 18:36:13 by gguardam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_env(t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	while (shell->env[i])
	{
		j = 0;
		while (shell->env[i][j])
		{
			write(shell->commands->out_fd, &shell->env[i][j], 1);
			j++;
		}
		write(shell->commands->out_fd, "\n", 1);
		i++;
	}
}
