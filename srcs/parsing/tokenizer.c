/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:19:52 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/10 11:01:31 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

t_token *create_token(char *value, t_token_type type)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->value = expand_env_var(value);
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

t_token *tokenize(char *input)
{
    t_token *tokens = NULL;
    t_token *last = NULL;
    int i = 0;

    while (input[i])
    {
        if (input[i] == ' ') // Ignore les espaces
        {
            i++;
            continue;
        }
        char buffer[1024];
        int j = 0;
        t_token_type type = WORD;

        if (is_operator(input[i])) // Gestion des opérateurs
        {
            buffer[j++] = input[i++];
            if (buffer[0] == '<' && input[i] == '<') { buffer[j++] = input[i++]; type = HEREDOC; }
            else if (buffer[0] == '>' && input[i] == '>') { buffer[j++] = input[i++]; type = APPEND; }
            else if (buffer[0] == '<') type = REDIR_IN;
            else if (buffer[0] == '>') type = REDIR_OUT;
            else if (buffer[0] == '|') type = PIPE;
        }
        else if (input[i] == '"' || input[i] == '\'') // Gestion des quotes
        {
            char quote = input[i++];
            while (input[i] && input[i] != quote)
                buffer[j++] = input[i++];
            if (input[i] == quote)
                i++;
            type = QUOTE;
        }
        else // Commandes, arguments, variables d’environnement
        {
            while (input[i] && !is_operator(input[i]) && input[i] != ' ')
                buffer[j++] = input[i++];
        }
        buffer[j] = '\0';
        t_token *new_token = create_token(buffer, type);
        if (!tokens)
            tokens = new_token;
        else
            last->next = new_token;
        last = new_token;
    }
    return tokens;
}

int is_in_simple_quotes(char *input, int pos)
{
    int i = 0;
    int in_single_quote = 0;

    while (i < pos)
    {
        if (input[i] == '\'')
            in_single_quote = !in_single_quote;
        i++;
    }
    return in_single_quote;
}
