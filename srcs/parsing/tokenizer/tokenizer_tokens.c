/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:06:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/15 09:32:21 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token	*create_token(char *value, t_token_type type,
	t_quote_type quote_type, t_data *data)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (quote_type == SINGLE_QUOTE || type == DELIMITER || data->from_quotes)
		token->value = ft_strdup(value);
	else
		token->value = expand_env_var(value, quote_type, data);
	token->type = type;
	token->quote_type = quote_type;
	token->from_quotes = data->from_quotes;
	token->next = NULL;
	return (token);
}

int	check_unclosed_quotes(char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
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
	return (quote != 0);
}

void	add_token(t_token **tokens, t_token **last,
					t_token_info info, t_data *data)
{
	t_token	*new_token;

	data->from_quotes = false;
	new_token = create_token(info.buffer, info.type,
			info.in_single_quotes, data);
	if (!(*tokens))
		*tokens = new_token;
	else
		(*last)->next = new_token;
	*last = new_token;
}
