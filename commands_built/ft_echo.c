/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axgimene <axgimene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:35:13 by gguardam          #+#    #+#             */
/*   Updated: 2025/11/17 18:45:17 by axgimene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_echo(t_cmd *cmd)
{
    int		i;
    int		newline;
    int		j;

    i = 1;
    newline = 1;
    while (cmd->av[i])
    {
        if (cmd->av[i][0] == '-' && cmd->av[i][1] == 'n')
        {
            j = 1;
            while (cmd->av[i][j] == 'n')
                j++;
            if (cmd->av[i][j] == '\0')
            {
                newline = 0;
                i++;
                continue;
            }
        }
        break;
    }
    while (cmd->av[i])
    {
        printf("%s", cmd->av[i]);
        if (cmd->av[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
}
