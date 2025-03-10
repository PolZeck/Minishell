/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:03:07 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/10 13:04:02 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

t_token *create_token(char *value, t_token_type type, int in_single_quotes)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    
    if (type == QUOTE && in_single_quotes)
        token->value = ft_strdup(value); // Pas d'expansion pour les quotes simples
    else
        token->value = expand_env_var(value, in_single_quotes);

    token->type = type;
    token->next = NULL;
    return (token);
}

int check_unclosed_quotes(char *input)
{
    int i = 0;
    char quote = 0;

    while (input[i])
    {
        if (input[i] == '"' || input[i] == '\'')
        {
            if (quote == 0)
                quote = input[i];
            else if (quote == input[i])
                quote = 0;
        }
        i++;
    }
    return (quote != 0); // Retourne 1 si une quote est restée ouverte
}