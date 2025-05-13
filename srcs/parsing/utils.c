/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:37:53 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/13 12:20:15 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == WORD || tokens->type == QUOTE)
			count++;
		else if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == HEREDOC || tokens->type == APPEND)
		{
			if (tokens->next)
				tokens = tokens->next;
		}
		tokens = tokens->next;
	}
	return (count);
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, ".") == 0)
		return (1);
	return (0);
}

void	syntax_error(char *token)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	*get_exit_status() = 2;
}
