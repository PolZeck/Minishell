/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:19:52 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/10 14:15:23 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token *tokenize(char *input)
{
    t_token *tokens = NULL;
    t_token *last = NULL;
    int i = 0;
    int in_single_quotes = 0;
    int in_double_quotes = 0;

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
			
			if (buffer[0] == '<')
			{
				if (input[i] == '<') // Gestion de "<<"
				{
					buffer[j++] = input[i++];
					type = HEREDOC;
				}
				else
					type = REDIR_IN; // Gestion de "<"
			}
			else if (buffer[0] == '>')
			{
				if (input[i] == '>') // Gestion de ">>"
				{
					buffer[j++] = input[i++];
					type = APPEND;
				}
				else
					type = REDIR_OUT; // Gestion de ">"
			}
			else if (buffer[0] == '|') // Gestion de "|"
				type = PIPE;
		}
        else if (input[i] == '"' || input[i] == '\'') // Gestion des quotes
        {
            char quote = input[i++];
            if (quote == '\'')
                in_single_quotes = !in_single_quotes;
            else
                in_double_quotes = !in_double_quotes;

            while (input[i] && input[i] != quote)
            {
                if (input[i] == '$' && !in_single_quotes) // Expansion des variables dans les doubles quotes
                {
                    char var_name[256];
                    int k = 0;
                    i++; // Passer le `$`
                    while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
                        var_name[k++] = input[i++];
                    var_name[k] = '\0';
                    
                    char *expanded = getenv(var_name);
                    if (expanded)
                    {
                        int l = 0;
                        while (expanded[l])
                            buffer[j++] = expanded[l++];
                    }
                }
                else
                {
                    buffer[j++] = input[i++];
                }
            }

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

        t_token *new_token = create_token(buffer, type, in_single_quotes);
        if (!tokens)
            tokens = new_token;
        else
            last->next = new_token;
        last = new_token;
    }
    return tokens;
}
