/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_parser_main4.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axgimene <axgimene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 09:26:02 by axgimene          #+#    #+#             */
/*   Updated: 2025/11/24 19:37:49 by axgimene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	process_word_token(t_token *current_token, t_cmd *current_cmd)
{
    add_arg_to_command(current_cmd, current_token->value);
    // NO avances el puntero aquí.
    return (1);
}

static int	process_redir_token(t_token **current_token, t_cmd **current_cmd)
{
    t_token	*redir_token;
    t_token	*filename_token;

    redir_token = *current_token;
    filename_token = redir_token->next;

    if (!filename_token || filename_token->type != T_WORD)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
        if (filename_token)
            ft_putstr_fd(filename_token->value, 2);
        else
            ft_putstr_fd("newline", 2);
        ft_putstr_fd("'\n", 2);
        *current_cmd = NULL;
        return (0);
    }
    if (!handle_redirection(current_token, *current_cmd))
    {
        *current_cmd = NULL;
        return (0);
    }
    // Avanza el puntero UNA VEZ para saltar el nombre de archivo.
    // El bucle principal saltará la redirección.
    *current_token = (*current_token)->next;
    return (1);
}

static int	process_token_in_parser(t_token **current_token,
    t_cmd **current_cmd)
{
    if (*current_cmd == NULL)
        return (0);
    if ((*current_token)->type == T_WORD)
        return (process_word_token(*current_token, *current_cmd)); // Pasa por valor
    else if ((*current_token)->type == T_PIPE)
        return (handle_pipe_token(current_token, current_cmd));
    else if (is_redirection_token((*current_token)->type))
        return (process_redir_token(current_token, current_cmd));
    
    return (1); // Para tokens desconocidos, simplemente avanza.
}

static int	process_all_tokens(t_token **current, t_cmd **current_cmd)
{
    while (*current)
    {
        if (!process_token_in_parser(current, current_cmd))
        {
            return (0);
        }
        // ✅ AVANZA EL PUNTERO AQUÍ, EN EL BUCLE PRINCIPAL
        if (*current) // Comprueba si no es NULL después de procesar
            *current = (*current)->next;
    }
    return (1);
}

t_cmd	*parse_tokens(t_token *tokens)
{
    t_cmd	*head;
    t_cmd	*current_cmd;
    t_token	*current;

    if (!tokens)
        return (NULL);
    current = tokens;
    head = NULL;
    init_first_command(&head, &current_cmd);
    if (!current_cmd)
    {
        // ✅ Si la creación del primer comando falla, no hay nada que liberar
        return (NULL);
    }
    
    if (!process_all_tokens(&current, &current_cmd))
    {
        free_commands(&head); // ✅ Libera la lista completa, incluyendo 'head'
        return (NULL);
    }
    if (!validate_final_command(current_cmd))
    {
        free_commands(&head); // ✅ Libera la lista completa, incluyendo 'head'
        return (NULL);
    }
    set_builtin_flag(current_cmd);
    return (head);
}
