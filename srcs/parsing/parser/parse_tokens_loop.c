/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_loop.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:33:37 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/19 15:38:03 by pol              ###   ########.fr       */
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
			if (!handle_pipe_token(&tokens, &cmd, &arg_count, data))
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
	int *arg_count, t_data *data)
{
	if (!(*tokens)->next || (*tokens)->next->type == PIPE)
	{
		ft_printf("bash: syntax error near unexpected token `|'\n");
		data->exit_status = 2;
		free_cmds(data->cmds_head);
		return (false);
	}
	(*tokens) = (*tokens)->next;
	(*cmd)->next = create_cmd(*tokens);
	if (!(*cmd)->next)
		return (free_cmds(data->cmds_head), false);
	(*cmd) = (*cmd)->next;
	*arg_count = 0;
	return (true);
}
