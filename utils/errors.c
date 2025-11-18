/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axgimene <axgimene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:37:37 by gguardam          #+#    #+#             */
/*   Updated: 2025/11/18 11:41:18 by axgimene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	freeing_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	error_executing\
(int site_of_error, char **env, char **cmd_params)
{
	int	i;

	i = 0;
	freeing_env(env);
	while (cmd_params[i])
	{
		free (cmd_params[i]);
		i++;
	}
	free (cmd_params);
	if (site_of_error == 0)
		exit((perror("command not found"), 127));
	else if (site_of_error == 1)
		exit((perror("bin not found"), 127));
	else
		exit((perror("execve"), 127));
}

void	write_error_message(int fd, char *cmd, char *arg, char *error_msg)
{
    ft_putstr_fd(cmd, fd);
    ft_putstr_fd(": ", fd);
    if (arg && arg[0] != '\0')
    {
        ft_putstr_fd(arg, fd);
        ft_putstr_fd(": ", fd);
    }
    ft_putstr_fd(error_msg, fd);
    ft_putstr_fd("\n", fd);
}
