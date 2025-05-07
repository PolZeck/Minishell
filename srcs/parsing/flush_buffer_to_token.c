/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flush_buffer_to_token.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:22:44 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 09:22:51 by pledieu          ###   ########lyon.fr   */
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
	type = (info && info->next_is_delimiter) ? DELIMITER : WORD;
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
	char **buffer, t_quote_type quote_type, t_parseinfo *info)
{
	t_token	*new;

	if (!*buffer)
		return ;
	new = create_token_from_buffer(buffer, quote_type, info);
	if (!new)
		return ;
	append_token(token_list, new);
	free(*buffer);
	*buffer = ft_strdup("");
}
