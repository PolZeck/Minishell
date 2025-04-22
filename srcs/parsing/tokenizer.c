/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:19:52 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/22 13:47:10 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_input_token(t_token_list *tlist,
	char **buffer, char *input, int *i)
{
	static t_quote_type current_quote_type = NO_QUOTE;

	if (input[*i] == ' ')
	{
		flush_buffer_to_token(tlist->tokens, tlist->last, buffer, current_quote_type);
		current_quote_type = NO_QUOTE;
		(*i)++;
		return ;
	}
	if (is_operator(input[*i]))
	{
		flush_buffer_to_token(tlist->tokens, tlist->last, buffer, current_quote_type);
		current_quote_type = NO_QUOTE;
		handle_operator_token(tlist->tokens, tlist->last, input, i);
		return ;
	}
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		handle_quotes_in_token(buffer, input, i, &current_quote_type);
		return ;
	}
	if (input[*i] == '$')
	{
		handle_variable_expansion(buffer, input, i);
		return ;
	}
	append_word(buffer, input, i);
}





t_token	*tokenize(char *input)
{
	t_token			*tokens;
	t_token			*last;
	t_token_list	tlist;
	char			*buffer;
	int				i;

	i = 0;
	tokens = NULL;
	last = NULL;
	tlist.tokens = &tokens;
	tlist.last = &last;
	buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	while (input[i])
		handle_input_token(&tlist, &buffer, input, &i);
	if (*buffer)
		flush_buffer_to_token(&tokens, &last, &buffer, NO_QUOTE);
	free(buffer);
	return (tokens);
}


void	handle_token(t_token **tokens, t_token **last, char *input, int *i)
{
	char			*buffer;
	int				j;
	t_token_info	info;
	t_quote			q;

	buffer = malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!buffer)
		return ;
	j = 0;
	info.type = WORD;
	info.in_single_quotes = 0;
	q.buffer = buffer;
	q.input = input;
	q.i = i;
	q.j = &j;
	if (is_operator(input[*i]))
		info.type = handle_operator(buffer, input, i, &j);
	else
		process_word_or_quote(&q, &info);
	buffer[j] = '\0';
	info.buffer = buffer;
	add_token(tokens, last, info);
	free(buffer);
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
