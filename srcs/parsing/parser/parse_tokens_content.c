/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_content.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:34:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 09:51:33 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	handle_redirections(t_cmd *cmd, t_token **tokens)
{
	if ((*tokens)->type == REDIR_IN)
		handle_redir_in(cmd, tokens);
	else if ((*tokens)->type == REDIR_OUT)
		handle_redir_out(cmd, tokens, 0);
	else if ((*tokens)->type == APPEND)
		handle_redir_out(cmd, tokens, 1);
	else if ((*tokens)->type == HEREDOC)
		handle_heredoc(cmd, tokens);
	if (cmd->invalid)
		return (0);
	return (1);
}

bool	handle_token_content(t_cmd *cmd, t_token **tokens,
	int *arg_count, t_cmd *head)
{
	char	**split;
	int		i;

	if ((*tokens)->type == WORD || (*tokens)->type == QUOTE
		|| (*tokens)->type == DELIMITER)
	{
		if (should_split_token(*tokens))
		{
			split = ft_split((*tokens)->value, ' ');
			i = 0;
			while (split && split[i])
			{
				if (split[i][0] != '\0')
					handle_argument(cmd, arg_count, split[i]);
				i++;
			}
			free_split(split);
		}
		else
			handle_argument(cmd, arg_count, (*tokens)->value);
	}
	else if (!handle_redirections(cmd, tokens))
		return (free_cmds(head), false);
	return (true);
}

bool	should_split_token(t_token *token)
{
	return (token->quote_type == NO_QUOTE
		&& token->type == WORD
		&& token->value[0] != '\0'
		&& !ft_strchr(token->value, '\'')
		&& !ft_strchr(token->value, '\"')
		&& ft_strchr(token->value, ' ')
		&& token->value[0] != ' ');
}
