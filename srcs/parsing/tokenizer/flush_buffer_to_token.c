/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flush_buffer_to_token.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:22:44 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/14 23:56:32 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_token	*create_token_from_buffer(char **buffer,
	t_quote_type quote_type, t_parseinfo *info)
{
	t_token			*new;
	t_token_type	type;

	if (!*buffer || (!ft_strlen(*buffer) && quote_type == NO_QUOTE))
		return (NULL);
	if (info && info->next_is_delimiter)
		type = DELIMITER;
	else
		type = WORD;
	info->next_is_delimiter = 0;
	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(*buffer);
	new->type = type;
	new->quote_type = quote_type;
	new->next = NULL;
	return (new);
}

static void	append_token(t_token_list token_list, t_token *new)
{
	if (!*(token_list.tokens))
		*(token_list.tokens) = new;
	else
		(*(token_list.last))->next = new;
	*(token_list.last) = new;
}

void	flush_buffer_to_token(t_token_list token_list,
	char **buffer, t_quote_type quote_type,
	t_parseinfo *info)
{
	t_token	*new;
	char	**words;
	int		i;
	bool	actual_from_quotes;

	if (!*buffer || !**buffer)
		return ;
	actual_from_quotes = info->came_from_quote || info->buffer_contains_quote;
	if (quote_type == NO_QUOTE && !actual_from_quotes)
	{
		words = ft_split(*buffer, ' ');
		if (!words)
			return ;
		i = 0;
		while (words[i])
		{
			char *tmp = ft_strdup(words[i]);
			new = create_token_from_buffer(&tmp, quote_type, info);
			free(tmp);
			append_token(token_list, new);
			i++;
		}
		free_split(words);
	}
	else
	{
		new = create_token(*buffer, WORD, quote_type, info->data, actual_from_quotes);
		append_token(token_list, new);
	}
	info->buffer_contains_quote = false;
	free(*buffer);
	*buffer = ft_strdup("");
}
