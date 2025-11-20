/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expander_utils7.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axgimene <axgimene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 12:00:50 by axgimene          #+#    #+#             */
/*   Updated: 2025/11/20 19:13:11 by axgimene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*handle_regular_char(char *str, int *i)
{
	char	*part;

	part = ft_substr(str, *i, 1);
	if (!part)
		return (ft_strdup(""));
	(*i)++;
	return (part);
}

static char	*process_char_in_expansion(t_shell *shell, char *str, int *i)
{
	if (str[*i] == '\'')
		return (handle_single_quotes(str, i));
	else if (str[*i] == '"')
		return (handle_double_quotes(shell, str, i));
	else if (str[*i] == '$' && str[*i + 1]
		&& !is_dollar_terminator(str[*i + 1]))
		return (expand_dollar(shell, str, i));
	else
		return (handle_regular_char(str, i));
}

char	*expand_string(t_shell *shell, char *str)
{
    int		i;
    char	*result;
    char	*temp;
    char	*part;
    char	*new_result;

    i = 0;
    result = ft_strdup("");
    if (!result)
        return (NULL);
    while (str && str[i])
    {
        part = process_char_in_expansion(shell, str, &i);
        if (!part)
            part = ft_strdup("");
        if (!part)  // ✅ Valida si ft_strdup falló
        {
            free(result);
            return (NULL);
        }
        temp = result;
        new_result = ft_strjoin(result, part);  // ✅ Usa variable temporal
        if (!new_result)  // ✅ Valida si ft_strjoin falló
        {
            free(temp);   // ← Libera result anterior
            free(part);
            return (NULL);
        }
        free(temp);
        free(part);
        result = new_result;
    }
    return (result);
}

void	expand_variables(t_shell *shell, t_token *tokens)
{
    t_token	*current;
    char	*expanded;

    current = tokens;
    while (current)
    {
        if (current->type == T_WORD)
        {
            expanded = expand_string(shell, current->value);
            if (expanded != NULL)  // ✅ Siempre valida
            {
                free(current->value);
                current->value = expanded;
            }
            else  // ✅ Si falla, mantén el valor anterior
            {
                // Opcional: puedes loguear error aquí
                // fprintf(stderr, "expand_string failed\n");
            }
        }
        current = current->next;
    }
}
