/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguardam <gguardam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:35:13 by gguardam          #+#    #+#             */
/*   Updated: 2025/11/10 18:35:16 by gguardam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	there_are_params(t_cmd *command)
{
	int	i;
	int	flag;
	int	j;

	i = 1;
	j = 2;
	flag = 1;
	while (command->av[i][j])
	{
		if (command->av[i][j] != 'n')
			flag = 0;
		j++;
	}
	if (flag)
		i += 1;
	while (command->av[i])
	{
		ft_putstr_fd(command->av[i], command->out_fd);
		if (command->av[i + 1])
			write(command->out_fd, " ", 1);
		i++;
	}
	if (!flag)
		write(command->out_fd, "\n", 1);
}

void	ft_echo(t_cmd *command)
{
	int	i;

	i = 1;
	if (!command->av[i])
		write(command->out_fd, "\n", 1);
	else if (command->av[i] && ft_strncmp(command->av[1], "-n", 2) == 0)
		there_are_params(command);
	else
	{
		while (command->av[i])
		{
			write(command->out_fd, command->av[i], ft_strlen(command->av[i]));
			if (command->av[i + 1])
				write(command->out_fd, " ", 1);
			i++;
		}
		write(command->out_fd, "\n", 1);
	}
}
