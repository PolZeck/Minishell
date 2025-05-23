/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 08:30:42 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/14 23:48:26 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	init_quote(char quote, t_parseinfo *info)
{
	if (quote == '\'' && *(info->quote_type) == NO_QUOTE)
		*(info->quote_type) = SINGLE_QUOTE;
	else if (quote == '\"' && *(info->quote_type) == NO_QUOTE)
		*(info->quote_type) = DOUBLE_QUOTE;
	(*(info->i))++;
}

static char	*handle_single_quote(t_parseinfo *info)
{
	int		start;

	start = *(info->i);
	while (info->input[*(info->i)] && info->input[*(info->i)] != '\'')
		(*(info->i))++;
	return (ft_substr(info->input, start, *(info->i) - start));
}

static void	handle_double_quote(char **sub, t_parseinfo *info)
{
	int		start;
	char	*tmp;
	char	*joined;

	*sub = ft_strdup("");
	while (info->input[*(info->i)] && info->input[*(info->i)] != '\"')
	{
		if (info->input[*(info->i)] == '$')
			handle_variable_expansion(sub, info->input, info->i, info);
		else
		{
			start = *(info->i);
			while (info->input[*(info->i)] && info->input[*(info->i)] != '$'
				&& info->input[*(info->i)] != '\"')
				(*(info->i))++;
			tmp = ft_substr(info->input, start, *(info->i) - start);
			joined = ft_strjoin(*sub, tmp);
			free(*sub);
			free(tmp);
			*sub = joined;
		}
	}
}

static void	merge_and_flush(char **buffer, char *sub, t_parseinfo *info,
	t_token_list token_list)
{
	char			*tmp;
	char			next;
	t_quote_type	current;

	tmp = ft_strjoin(*buffer, sub);
	free(*buffer);
	free(sub);
	*buffer = tmp;
	info->buffer_contains_quote = true;
	next = info->input[*(info->i)];
	current = *(info->quote_type);
	*(info->quote_type) = NO_QUOTE;
	if (next == '\0' || next == ' ' || is_operator(next))
	{
		info->came_from_quote = true;
		flush_buffer_to_token(token_list, buffer, current, info);
	}
}

void	handle_quotes_in_token(char **buffer, t_parseinfo *info,
	t_token **tokens, t_token **last)
{
	char			quote;
	char			*sub;
	t_token_list	token_list;

	quote = info->input[*(info->i)];
	init_quote(quote, info);
	if (quote == '\'')
		sub = handle_single_quote(info);
	else
		handle_double_quote(&sub, info);
	if (info->input[*(info->i)] == quote)
		(*(info->i))++;
	token_list.tokens = tokens;
	token_list.last = last;
	merge_and_flush(buffer, sub, info, token_list);
}
