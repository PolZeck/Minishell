/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:36:04 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 09:57:37 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	finalize_args(t_cmd *head)
{
	int		i;
	t_cmd	*cur;

	cur = head;
	while (cur)
	{
		if (cur->args)
		{
			i = 0;
			while (cur->args[i])
				i++;
			cur->args[i] = NULL;
		}
		cur = cur->next;
	}
}

t_cmd	*create_cmd(t_token *tokens)
{
	t_cmd	*cmd;
	int		i;
	int		arg_count;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	arg_count = count_args(tokens);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	i = 0;
	while (i <= arg_count)
	{
		cmd->args[i] = NULL;
		i++;
	}
	cmd->redirs = NULL;
	cmd->invalid = 0;
	cmd->next = NULL;
	return (cmd);
}
