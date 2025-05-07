/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:19:52 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 10:07:02 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_input_token(t_token_list *tlist,
	char **buffer, t_parseinfo *info)
{
	static t_quote_type	current_quote_type;
	char				next;

	if (info->input[*(info->i)] == ' ')
	{
		flush_buffer_to_token(*tlist, buffer, current_quote_type, info);
		current_quote_type = NO_QUOTE;
		(*(info->i))++;
		return ;
	}
	if (is_operator(info->input[*(info->i)]))
	{
		flush_buffer_to_token(*tlist, buffer, current_quote_type, info);
		current_quote_type = NO_QUOTE;
		info->quote = 0;
		handle_operator_token(tlist->tokens, tlist->last, info);
		return ;
	}
	if (info->input[*(info->i)] == '\'' || info->input[*(info->i)] == '\"')
	{
		info->quote_type = &current_quote_type;
		handle_quotes_in_token(buffer, info, tlist->tokens, tlist->last);
		return ;
	}
	if (info->input[*(info->i)] == '$')
	{
		next = info->input[*(info->i) + 1];
		if (next == '\'' || next == '\"')
		{
			(*(info->i))++;
			return ;
		}
		handle_variable_expansion(buffer, info->input, info->i, info);
		return ;
	}
	append_word(buffer, info->input, info->i);
}

t_token	*tokenize(char *input, t_data *data)
{
	t_token			*tokens;
	t_token			*last;
	t_token_list	tlist;
	char			*buffer;
	int				i;
	t_parseinfo		info;

	i = 0;
	tokens = NULL;
	last = NULL;
	tlist.tokens = &tokens;
	tlist.last = &last;
	buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	info.data = data;
	info.input = input;
	info.i = &i;
	info.quote = 0;
	info.quote_type = NULL;
	info.next_is_delimiter = 0;
	while (input[i])
		handle_input_token(&tlist, &buffer, &info);
	if (*buffer)
		flush_buffer_to_token(tlist, &buffer, NO_QUOTE, &info);
	free(buffer);
	return (tokens);
}

void	process_word_or_quote(t_quote *q, t_token_info *info)
{
	if (q->input[*q->i] == '"' || q->input[*q->i] == '\'')
		info->type = handle_quotes(q);
	else
	{
		while (q->input[*q->i] && !is_operator(q->input[*q->i])
			&& q->input[*q->i] != ' ')
			q->buffer[(*q->j)++] = q->input[(*q->i)++];
	}
}

t_token_type	handle_operator(char *buffer, char *input, int *i, int *j)
{
	t_token_type	type;

	buffer[(*j)++] = input[(*i)++];
	if (buffer[0] == '<' || buffer[0] == '>')
		type = handle_redirections_token(buffer, input, i, j);
	else
		type = PIPE;
	return (type);
}
