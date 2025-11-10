/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguardam <gguardam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:37:37 by gguardam          #+#    #+#             */
/*   Updated: 2025/11/10 19:16:14 by gguardam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

// Función auxiliar para escribir mensajes de error usando el FD correcto
void	write_error_message(int fd, char *cmd, char *arg, char *msg)
{
	write(fd, "bash: ", 6);
	write(fd, cmd, ft_strlen(cmd));
	write(fd, ": `", 3);
	write(fd, arg, ft_strlen(arg));
	write(fd, "': ", 3);
	write(fd, msg, ft_strlen(msg));
	write(fd, "\n", 1);
}
