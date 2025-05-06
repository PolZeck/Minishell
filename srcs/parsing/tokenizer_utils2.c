/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:45:23 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/06 15:45:31 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	flush_buffer_to_token(t_token **tokens, t_token **last,
		char **buffer, t_quote_type quote_type, t_parseinfo *info)
{
	t_token			*new;
	t_token_type	final_type;

	if (!*buffer)
		return ;
	if ((!*buffer || ft_strlen(*buffer) == 0) && quote_type == NO_QUOTE)
		return ;
	info->next_is_delimiter = 0;
	final_type = WORD;
	if (info && info->next_is_delimiter)
	{
		final_type = DELIMITER;
		info->next_is_delimiter = 0;
	}
	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = ft_strdup(*buffer);
	new->type = final_type;
	new->quote_type = quote_type;
	new->next = NULL;
	if (!*tokens)
		*tokens = new;
	else
		(*last)->next = new;
	*last = new;
	free(*buffer);
	*buffer = ft_strdup("");
}

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
	t_token **tokens_and_last[2])
{
	char	*tmp;
	char	next;

	if (!(sub[0] == '\0' && (*buffer)[0] != '\0'))
	{
		tmp = ft_strjoin(*buffer, sub);
		free(*buffer);
		free(sub);
		*buffer = tmp;
	}
	else
		free(sub);
	next = info->input[*(info->i)];
	if (next == '\0' || next == ' ' || is_operator(next))
		flush_buffer_to_token(tokens_and_last[0], tokens_and_last[1],
			buffer, *(info->quote_type), info);
	*(info->quote_type) = NO_QUOTE;
}

void	handle_quotes_in_token(char **buffer, t_parseinfo *info,
	t_token **tokens, t_token **last)
{
	char	quote;
	char	*sub;
	t_token	**tokens_and_last[2];

	quote = info->input[*(info->i)];
	init_quote(quote, info);
	if (quote == '\'')
		sub = handle_single_quote(info);
	else
		handle_double_quote(&sub, info);
	if (info->input[*(info->i)] == quote)
		(*(info->i))++;
	tokens_and_last[0] = tokens;
	tokens_and_last[1] = last;
	merge_and_flush(buffer, sub, info, tokens_and_last);
}

static void	append_raw_variable(char **buffer, char *input, int *i)
{
	int		start;
	char	*raw;
	char	*tmp;

	start = *i;
	(*i)++;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	raw = ft_substr(input, start, *i - start);
	tmp = ft_strjoin(*buffer, raw);
	free(*buffer);
	free(raw);
	*buffer = tmp;
}

static int	handle_digit_expansion(char **buffer, char *input, int *i)
{
	char	tmp_str[2];
	char	*joined;

	if (!ft_isdigit(input[*i]))
		return (0);
	(*i)++;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
	{
		tmp_str[0] = input[*i];
		tmp_str[1] = '\0';
		joined = ft_strjoin(*buffer, tmp_str);
		free(*buffer);
		*buffer = joined;
		(*i)++;
	}
	return (1);
}

static char	*extract_variable(char *input, int *i)
{
	int		start;
	char	*var_name;
	char	*var;

	if (input[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("$?"));
	}
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var_name = ft_substr(input, start, *i - start);
	var = ft_strjoin("$", var_name);
	free(var_name);
	return (var);
}

void	handle_variable_expansion(char **buffer,
	char *input, int *i, t_parseinfo *info)
{
	char	*var;
	char	*value;
	char	*to_append;
	char	*tmp;

	if (info && info->next_is_delimiter)
		return (append_raw_variable(buffer, input, i));
	if (input[*i] == '"' || input[*i] == '\'')
		return ;
	(*i)++;
	if (handle_digit_expansion(buffer, input, i))
		return ;
	var = extract_variable(input, i);
	value = expand_env_var(var, 0, info->data);
	to_append = ft_strdup(value);
	tmp = ft_strjoin(*buffer, to_append);
	free(*buffer);
	free(var);
	free(to_append);
	free(value);
	*buffer = tmp;
}

void	append_word(char **buffer, char *input, int *i)
{
	int		start;
	char	*word;
	char	*tmp;

	start = *i;
	while (input[*i] && !is_operator(input[*i])
		&& input[*i] != ' ' && input[*i] != '$'
		&& input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	word = ft_substr(input, start, *i - start);
	tmp = ft_strjoin(*buffer, word);
	free(*buffer);
	free(word);
	*buffer = tmp;
}
