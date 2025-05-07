/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_operators.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:03:24 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 11:10:16 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	handle_operator_token(t_token **tokens,
	t_token **last, t_parseinfo *info)
{
	char			op[3];
	int				j;
	t_token_type	type;
	t_token			*new;

	j = 0;
	op[j++] = info->input[(*info->i)++];
	if ((op[0] == '<' || op[0] == '>') && info->input[(*info->i)] == op[0])
		op[j++] = info->input[(*info->i)++];
	op[j] = '\0';
	type = get_token_type_from_op(op);
	new = create_token(op, type, NO_QUOTE, info->data);
	if (!*tokens)
		*tokens = new;
	else
		(*last)->next = new;
	*last = new;
	if (type == HEREDOC)
		info->next_is_delimiter = 1;
	else
		info->next_is_delimiter = 0;
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

t_token_type	get_token_type_from_op(char *op)
{
	if (ft_strcmp(op, "|") == 0)
		return (PIPE);
	if (ft_strcmp(op, "<") == 0)
		return (REDIR_IN);
	if (ft_strcmp(op, ">") == 0)
		return (REDIR_OUT);
	if (ft_strcmp(op, ">>") == 0)
		return (APPEND);
	if (ft_strcmp(op, "<<") == 0)
		return (HEREDOC);
	return (WORD);
}
