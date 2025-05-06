/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:03:07 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/06 14:56:54 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(char *value, t_token_type type,
	t_quote_type quote_type, t_data *data)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (type == DELIMITER || quote_type == SINGLE_QUOTE)
		token->value = ft_strdup(value);
	else
		token->value = expand_env_var(value, quote_type, data);
	token->type = type;
	token->quote_type = quote_type;
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

	new_token = create_token(info.buffer, info.type,
			info.in_single_quotes, data);
	if (!(*tokens))
		*tokens = new_token;
	else
		(*last)->next = new_token;
	*last = new_token;
}

t_token_type	handle_redirections_token(char *buffer,
												char *input, int *i, int *j)
{
	t_token_type	type;

	if (input[*i] == buffer[0])
	{
		buffer[(*j)++] = input[(*i)++];
		if (buffer[0] == '<')
			type = HEREDOC;
		else
			type = APPEND;
	}
	else
	{
		if (buffer[0] == '<')
			type = REDIR_IN;
		else
			type = REDIR_OUT;
	}
	return (type);
}

t_token_type	handle_quotes(t_quote *q)
{
	char			quote;
	t_token_type	type;

	quote = q->input[(*q->i)++];
	q->in_single_quotes = (quote == '\'');
	while (q->input[*q->i] && q->input[*q->i] != quote)
	{
		if (q->input[*q->i] == '$' && !(q->in_single_quotes))
			handle_expansion(q->buffer, q->input, q->i, q->j);
		else
			q->buffer[(*q->j)++] = q->input[(*q->i)++];
	}
	if (q->input[*q->i] == quote)
		(*q->i)++;
	type = QUOTE;
	return (type);
}
