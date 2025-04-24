/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:45:23 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/24 11:05:06 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	flush_buffer_to_token(t_token **tokens, t_token **last, char **buffer, t_quote_type quote_type)
{
	t_token	*new;

	if (!*buffer || !**buffer)
		return ;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = ft_strdup(*buffer); // contenu brut du buffer
	new->type = WORD;
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
	t_token **last, char *input, int *i, t_data *data)
{
	char			op[3];
	int				j;
	t_token_type	type;
	t_token			*new;

	j = 0;
	op[j++] = input[(*i)++];
	if ((op[0] == '<' || op[0] == '>') && input[*i] == op[0])
		op[j++] = input[(*i)++];
	op[j] = '\0';
	type = get_token_type_from_op(op);
	new = create_token(op, type, NO_QUOTE, data);
	if (!*tokens)
		*tokens = new;
	else
		(*last)->next = new;
	*last = new;
}

void	handle_quotes_in_token(char **buffer, char *input, int *i, t_quote_type *quote_type, t_data *data)
{
	char	quote;
	int		start;
	char	*sub;
	char	*tmp;

	quote = input[*i];
	if (quote == '\'' && *quote_type == NO_QUOTE)
		*quote_type = SINGLE_QUOTE;
	else if (quote == '\"' && *quote_type == NO_QUOTE)
		*quote_type = DOUBLE_QUOTE;

	(*i)++;

	if (quote == '\'')
	{
		start = *i;
		while (input[*i] && input[*i] != quote)
			(*i)++;
		sub = ft_substr(input, start, *i - start);
	}
	else
	{
		sub = ft_strdup("");
		while (input[*i] && input[*i] != quote)
		{
			if (input[*i] == '$')
				handle_variable_expansion(&sub, input, i, data);
			else
			{
				start = *i;
				while (input[*i] && input[*i] != '$' && input[*i] != quote)
					(*i)++;
				tmp = ft_substr(input, start, *i - start);
				sub = ft_strjoin(sub, tmp);
			}
		}
	}

	if (input[*i] == quote)
		(*i)++;

	tmp = ft_strjoin(*buffer, sub);
	free(*buffer);
	free(sub);
	*buffer = tmp;
}





void	handle_variable_expansion(char **buffer, char *input, int *i, t_data *data)
{
	int		start;
	char	*var;
	char	*value;
	char	*to_append;
	char	*tmp;

	(*i)++; // saute le $
	if (input[*i] == '?') // cas spécial $?
	{
		var = ft_strdup("$?");
		(*i)++;
	}
	else
	{
		start = *i;
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
			(*i)++;
		var = ft_strjoin("$", ft_substr(input, start, *i - start));
	}

	value = expand_env_var(var, 0, data); // appelle ta fonction avec $ inclus
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
