/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:45:23 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 09:29:09 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
