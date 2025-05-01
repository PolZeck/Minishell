/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:45:23 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/01 12:15:29 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void flush_buffer_to_token(t_token **tokens, t_token **last, char **buffer, t_quote_type quote_type, t_parseinfo *info)
{
	t_token			*new;
	t_token_type	final_type;

	if (!*buffer)
		return ;
	if (!**buffer && quote_type == NO_QUOTE)
		return ;

	// ✅ Gestion du HEREDOC → DELIMITER
	final_type = WORD;
	if (info && info->next_is_delimiter)
	{
		final_type = DELIMITER;
		info->next_is_delimiter = 0;
	}

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = ft_strdup(*buffer); // même si buffer est "", on duplique ""
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
	if (type == HEREDOC)
		info->next_is_delimiter = 1;
	new = create_token(op, type, NO_QUOTE, info->data);
	if (!*tokens)
		*tokens = new;
	else
		(*last)->next = new;
	*last = new;
}

void	handle_quotes_in_token(char **buffer, t_parseinfo *info)
{
	char	quote;
	int		start;
	char	*sub;
	char	*tmp;

	quote = info->input[*(info->i)];
	if (quote == '\'' && *(info->quote_type) == NO_QUOTE)
		*(info->quote_type) = SINGLE_QUOTE;
	else if (quote == '\"' && *(info->quote_type) == NO_QUOTE)
		*(info->quote_type) = DOUBLE_QUOTE;
	(*(info->i))++;
	if (quote == '\'')
	{
		start = *(info->i);
		while (info->input[*(info->i)] && info->input[*(info->i)] != quote)
			(*(info->i))++;
		sub = ft_substr(info->input, start, *(info->i) - start);
	}
	else
	{
		sub = ft_strdup("");
		while (info->input[*(info->i)] && info->input[*(info->i)] != quote)
		{
			if (info->input[*(info->i)] == '$')
				handle_variable_expansion(&sub, info->input, info->i, info->data, info);
			else
			{
				start = *(info->i);
				while (info->input[*(info->i)] && info->input[*(info->i)] != '$'
					&& info->input[*(info->i)] != quote)
					(*(info->i))++;
				tmp = ft_substr(info->input, start, *(info->i) - start);
				char *joined = ft_strjoin(sub, tmp);
				free(sub);
				free(tmp); // ✅ ici tu libères la mémoire
				sub = joined;
			}
		}
	}
	if (info->input[*(info->i)] == quote)
		(*(info->i))++;
	tmp = ft_strjoin(*buffer, sub);
	free(*buffer);
	free(sub);
	*buffer = tmp;
}


void	handle_variable_expansion(char **buffer,
	char *input, int *i, t_data *data, t_parseinfo *info)
{
	int		start;
	char	*var;
	char	*var_name;
	char	*value;
	char	*to_append;
	char	*tmp;

	if (info && info->next_is_delimiter)
	{
		// Copie brute du nom de la variable sans expansion
		int start = *i;
		(*i)++; // skip '$'
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;
		char *raw = ft_substr(input, start, *i - start);
		char *tmp = ft_strjoin(*buffer, raw);
		free(*buffer);
		free(raw);
		*buffer = tmp;
		return ;
	}
	if (input[*i] == '"' || input[*i] == '\'')
		return ; // on ne fait pas d'expansion, on laisse la quote être gérée ailleurs
	(*i)++;
	if (input[*i] == '?')
	{
		var = ft_strdup("$?");
		(*i)++;
	}
	else
	{
		start = *i;
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;
		var_name = ft_substr(input, start, *i - start);     // ✅ ALLOC
		var = ft_strjoin("$", var_name);                    // ✅ ALLOC
		free(var_name);                                     // ✅ LIBÉRÉ
	}
	value = expand_env_var(var, 0, data);                   // ✅ PEUT FAIRE strdup
	to_append = ft_strdup(value);                           // ✅ ALLOC
	tmp = ft_strjoin(*buffer, to_append);                   // ✅ ALLOC
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
