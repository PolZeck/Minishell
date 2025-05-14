/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:41:38 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/14 23:47:36 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static bool	handle_quote(t_parseinfo *info, char **buffer,
	t_token_list *tlist, t_quote_type *quote_type)
{
	if (info->input[*(info->i)] != '\'' && info->input[*(info->i)] != '\"')
		return (false);
	info->quote_type = quote_type;
	handle_quotes_in_token(buffer, info, tlist->tokens, tlist->last);
	return (true);
}

static bool	handle_operator_token_lexing(t_token_list *tlist,
	char **buffer, t_parseinfo *info, t_quote_type *quote_type)
{
	if (!is_operator(info->input[*(info->i)]))
		return (false);
	info->came_from_quote = false;
	flush_buffer_to_token(*tlist, buffer, *quote_type, info);
	*quote_type = NO_QUOTE;
	info->quote = 0;
	handle_operator_token(tlist->tokens, tlist->last, info);
	return (true);
}

static bool	handle_whitespace(t_token_list *tlist, char **buffer,
	t_parseinfo *info, t_quote_type *quote_type)
{
	if (info->input[*(info->i)] != ' ')
		return (false);
	info->came_from_quote = false;
	flush_buffer_to_token(*tlist, buffer, *quote_type, info);
	*quote_type = NO_QUOTE;
	(*(info->i))++;
	return (true);
}

void	handle_input_token(t_token_list *tlist,
	char **buffer, t_parseinfo *info)
{
	static t_quote_type	current_quote_type;
	char				next;

	if (handle_whitespace(tlist, buffer, info, &current_quote_type))
		return ;
	if (handle_operator_token_lexing(tlist, buffer, info, &current_quote_type))
		return ;
	if (handle_quote(info, buffer, tlist, &current_quote_type))
		return ;
	if (info->input[*(info->i)] == '$')
	{
		next = info->input[*(info->i) + 1];
		if (next == '\'' || next == '\"')
		{
			(*(info->i))++;
			return ;
		}
		handle_variable_expansion_tokenizer(tlist, buffer, info);
		return ;
	}
	append_word(buffer, info->input, info->i);
}
