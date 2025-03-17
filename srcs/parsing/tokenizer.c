/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:19:52 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/12 11:33:36 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	t_token	*last;
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;

	tokens = NULL;
	last = NULL;
	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (input[i])
	{
		if (input[i] == ' ')
			i++;
		else
			handle_token(&tokens, &last, input, &i);
	}
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

