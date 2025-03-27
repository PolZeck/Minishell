/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 13:22:40 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/27 16:06:15 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
