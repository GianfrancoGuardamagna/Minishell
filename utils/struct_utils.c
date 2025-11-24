/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axgimene <axgimene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 18:37:43 by gguardam          #+#    #+#             */
/*   Updated: 2025/11/24 20:17:03 by axgimene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_shell(t_shell *shell, char **envp)
{
    if (!shell)
        return ;
    shell->env = envp;
    shell->local_vars = NULL;
    shell->tokens = NULL;
    shell->commands = NULL;
    shell->prompt = NULL; // ✅ AÑADE ESTA LÍNEA
    shell->exit_status = 0;
    shell->stdin_copy = STDIN_FILENO;
    shell->stdout_copy = STDOUT_FILENO;
}

void	cleanup_shell(t_shell *shell)
{
    if (!shell)
        return ;
    
    // ✅ Libera el prompt (seguro si se llama múltiples veces)
    if (shell->prompt)
    {
        free(shell->prompt);
        shell->prompt = NULL;
    }
    
    // ✅ Libera tokens (seguro si se llama múltiples veces)
    if (shell->tokens)
    {
        free_tokens(&shell->tokens);
        shell->tokens = NULL;
    }
    
    // ✅ Libera commands (seguro si se llama múltiples veces)
    if (shell->commands)
    {
        free_commands(&shell->commands);
        shell->commands = NULL;
    }
    
    // ✅ Libera local_vars si existen (seguro si se llama múltiples veces)
    if (shell->local_vars)
    {
        int i = 0;
        while (shell->local_vars[i])
        {
            free(shell->local_vars[i]);
            shell->local_vars[i] = NULL;
            i++;
        }
        free(shell->local_vars);
        shell->local_vars = NULL;
    }
}

void	check_struct(t_shell *shell)
{
    int i;
    t_cmd *current;

    i = 0;
    printf("Stdin: %d\n", shell->stdin_copy);
    printf("Stdout: %d\n", shell->stdout_copy);
    printf("Exit Status: %d\n", shell->exit_status);
    printf("/////////////////////Commands///////////////////////////\n");
    current = shell->commands;  // Usar una variable temporal
    while(current)
    {
        printf("FDdin: %d\n", current->in_fd);
        printf("FDout: %d\n", current->out_fd);
        printf("Pipe 0: %d\n", current->pipe[0]);
        printf("Pipe 1: %d\n", current->pipe[1]);
        printf("Is Built in: %d\n", current->is_builtin);
        i = 0;
        if (current->av)  // Verificar que av no sea NULL
        {
            while(current->av[i])
            {
                printf("As %d: %s\n", i, current->av[i]);
                i++;
            }
        }
        printf("////////////////////////////////////////////////\n");
        current = current->next;  // Mover solo la variable temporal
    }
}