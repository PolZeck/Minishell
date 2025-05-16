/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_loop.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:33:37 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/16 14:30:14 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

bool	parse_all_tokens(t_token *tokens, t_cmd *cmd, t_cmd *head, t_data *data)
{
	int	arg_count;

	arg_count = 0;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!handle_pipe_token(&tokens, &cmd, &arg_count, head))
				return (false);
			continue ;
		}
		data->cmds_head = head;
		if (!handle_token_content(cmd, &tokens, &arg_count, data))
			return (false);
		if (cmd->invalid)
			return (free_cmds(head), false);
		tokens = tokens->next;
	}
	if (cmd && cmd->args)
		cmd->args[arg_count] = NULL;
	return (true);
}

bool	handle_pipe_token(t_token **tokens, t_cmd **cmd,
	int *arg_count, t_cmd *head)
{
	if (!(*tokens)->next || (*tokens)->next->type == PIPE)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
		*get_exit_status() = 2;
		free_cmds(head);
		return (false);
	}
	(*tokens) = (*tokens)->next;
	(*cmd)->next = create_cmd(*tokens);
	if (!(*cmd)->next)
		return (free_cmds(head), false);
	(*cmd) = (*cmd)->next;
	*arg_count = 0;
	return (true);
}
