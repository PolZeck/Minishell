/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:05:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 11:06:20 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	handle_expansion(char *buffer, char *input, int *i, int *j)
{
	char	*var_name;
	int		start;
	int		k;
	char	*expanded;

	start = *i + 1;
	while (input[start] && (ft_isalnum(input[start]) || input[start] == '_'))
		start++;
	var_name = malloc(sizeof(char) * (start - *i));
	if (!var_name)
		return ;
	k = 0;
	(*i)++;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		var_name[k++] = input[(*i)++];
	var_name[k] = '\0';
	expanded = getenv(var_name);
	free(var_name);
	if (expanded)
	{
		k = 0;
		while (expanded[k])
			buffer[(*j)++] = expanded[k++];
	}
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
