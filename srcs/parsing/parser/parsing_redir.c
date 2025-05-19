/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:09:49 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/19 13:38:51 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	handle_redir_in(t_cmd *cmd, t_token **tokens, t_data *data)
{
	*tokens = (*tokens)->next;
	if (!(*tokens))
	{
		syntax_error("newline", data);
		cmd->invalid = 1;
		return ;
	}
	if ((*tokens)->type != WORD && (*tokens)->type != DELIMITER)
	{
		if ((*tokens)->type == REDIR_IN || (*tokens)->type == REDIR_OUT
			|| (*tokens)->type == APPEND || (*tokens)->type == HEREDOC
			|| (*tokens)->type == PIPE)
			syntax_error("newline", data);
		else
			syntax_error((*tokens)->value, data);
		cmd->invalid = 1;
		return ;
	}
	add_redir(cmd, REDIR_IN, ft_strdup((*tokens)->value));
}

void	handle_redir_out(t_cmd *cmd, t_token **tokens, int append, t_data *data)
{
	t_token	*token;

	token = *tokens;
	if (token->next && (token->next->type == WORD
			|| token->next->type == DELIMITER))
	{
		if (append)
			add_redir(cmd, APPEND, ft_strdup(token->next->value));
		else
			add_redir(cmd, REDIR_OUT, ft_strdup(token->next->value));
		*tokens = token->next;
	}
	else
	{
		if (!token->next)
			syntax_error("newline", data);
		else
			syntax_error(token->next->value, data);
		cmd->invalid = 1;
	}
}
