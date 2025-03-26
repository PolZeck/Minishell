/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:19:52 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/26 12:21:45 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*tokenize(char *input)
{
	t_token	*tokens = NULL;
	t_token	*last = NULL;
	int		i = 0;
	int		start;
	char	*buffer = ft_strdup("");
	char	*temp;

	if (!buffer)
		return (NULL);
	while (input[i])
	{
		// Flush du buffer avant un espace
		if (input[i] == ' ')
		{
			if (*buffer)
			{
				t_token *new_token = create_token(buffer, WORD, 0);
				if (!tokens)
					tokens = new_token;
				else
					last->next = new_token;
				last = new_token;
				free(buffer);
				buffer = ft_strdup("");
				if (!buffer)
					return (NULL);
			}
			i++;
			continue;
		}

		// Flush du buffer si on va traiter un opérateur
		if (is_operator(input[i]) && *buffer)
		{
			t_token *new_token = create_token(buffer, WORD, 0);
			if (!tokens)
				tokens = new_token;
			else
				last->next = new_token;
			last = new_token;
			free(buffer);
			buffer = ft_strdup("");
			if (!buffer)
				return (NULL);
		}

		// Gestion d’un opérateur
		if (is_operator(input[i]))
		{
			t_token_type type;
			char op[3] = {0};
			op[0] = input[i++];
			if ((op[0] == '<' || op[0] == '>') && input[i] == op[0])
				op[1] = input[i++];
			type = get_token_type_from_op(op);
			t_token *op_token = create_token(op, type, 0);
			if (!tokens)
				tokens = op_token;
			else
				last->next = op_token;
			last = op_token;
			continue;
		}

		// Gestion des guillemets
		if (input[i] == '"' || input[i] == '\'')
		{
			char quote = input[i++];
			start = i;
			while (input[i] && input[i] != quote)
				i++;
			char *quoted = ft_substr(input, start, i - start);
			if (input[i] == quote)
				i++;
			temp = ft_strjoin(buffer, quoted);
			free(buffer);
			free(quoted);
			buffer = temp;
			continue;
		}

		// Gestion d’une variable $VAR
		if (input[i] == '$')
		{
			int var_start = ++i;
			while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
				i++;
			char *var_name = ft_substr(input, var_start, i - var_start);
			char *expanded = getenv(var_name);
			free(var_name);
			temp = ft_strjoin(buffer, expanded ? expanded : "");
			free(buffer);
			buffer = temp;
			continue;
		}

		// Lecture de mot normal
		start = i;
		while (input[i] && !is_operator(input[i])
			&& input[i] != ' ' && input[i] != '$' && input[i] != '"' && input[i] != '\'')
			i++;
		char *chunk = ft_substr(input, start, i - start);
		temp = ft_strjoin(buffer, chunk);
		free(buffer);
		free(chunk);
		buffer = temp;
	}

	// Dernier flush s’il reste du contenu
	if (*buffer)
	{
		t_token *new_token = create_token(buffer, WORD, 0);
		if (!tokens)
			tokens = new_token;
		else
			last->next = new_token;
		last = new_token;
	}
	free(buffer);
	return tokens;
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

