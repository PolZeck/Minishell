/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_words.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:02:06 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/14 23:42:35 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	handle_variable_expansion_tokenizer(t_token_list *tlist,
	char **buffer, t_parseinfo *info)
{
	char	*var = NULL;
	char	*value = NULL;
	char	*tmp;
	int		start;

	(void)tlist;

	// ✅ Cas spécial : on ne doit PAS expandre une variable si c'est un DELIMITER
	if (info->next_is_delimiter)
	{
		int i = *(info->i);
		int len = 1;

		while (info->input[i + len] && (ft_isalnum(info->input[i + len]) || info->input[i + len] == '_'))
			len++;

		char *raw = ft_substr(info->input, i, len);
		tmp = ft_strjoin(*buffer, raw);
		free(*buffer);
		*buffer = tmp;
		free(raw);
		*(info->i) += len;

		// ✅ important : consommer le flag
		info->next_is_delimiter = 0;
		return ;
	}

	// 🧠 Cas $ tout seul ou suivi de caractère non variable
	if (!info->input[*(info->i) + 1]
		|| (!ft_isalnum(info->input[*(info->i) + 1])
			&& info->input[*(info->i) + 1] != '_'
			&& info->input[*(info->i) + 1] != '?'))
	{
		tmp = ft_strjoin(*buffer, "$");
		free(*buffer);
		*buffer = tmp;
		(*info->i)++;
		return ;
	}

	(*info->i)++;

	if (info->input[*(info->i)] == '?')
	{
		value = ft_itoa(*get_exit_status());
		(*info->i)++;
	}
	else
	{
		start = *(info->i);
		while (ft_isalnum(info->input[*(info->i)]) || info->input[*(info->i)] == '_')
			(*info->i)++;
		var = ft_substr(info->input, start, *(info->i) - start);
		value = ft_getenv(info->data, var);
		if (value)
			value = ft_strdup(value);
		else
			value = ft_strdup("");
		free(var);
	}

	// Expansion dans les quotes
	if (info->quote_type && *(info->quote_type) != NO_QUOTE)
	{
		tmp = ft_strjoin(*buffer, value);
		free(*buffer);
		*buffer = tmp;
		free(value);
		return ;
	}

	// Expansion hors quotes
	tmp = ft_strjoin(*buffer, value);
	free(*buffer);
	*buffer = tmp;
	free(value);
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
