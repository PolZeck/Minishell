/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_content.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:34:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/15 01:17:29 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	handle_redirections(t_cmd *cmd, t_token **tokens, t_data *data)
{
	if ((*tokens)->type == REDIR_IN)
		handle_redir_in(cmd, tokens);
	else if ((*tokens)->type == REDIR_OUT)
		handle_redir_out(cmd, tokens, 0);
	else if ((*tokens)->type == APPEND)
		handle_redir_out(cmd, tokens, 1);
	else if ((*tokens)->type == HEREDOC)
		handle_heredoc(cmd, tokens, data);
	if (cmd->invalid)
		return (0);
	return (1);
}

bool	handle_token_content(t_cmd *cmd, t_token **tokens,
	int *arg_count, t_data	*data)
{
	char	**split;
	int		i;

	if ((*tokens)->type == WORD || (*tokens)->type == QUOTE
		|| (*tokens)->type == DELIMITER)
	{
		if (should_split_token(*tokens))
		{
			split = ft_split_charset((*tokens)->value, " \t");
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
	else if (!handle_redirections(cmd, tokens, data))
		return (free_cmds(data->cmds_head), false);
	return (true);
}

bool	should_split_token(t_token *token)
{
	if (token->from_quotes)
		return (false);
	if (token->quote_type != NO_QUOTE)
		return (false);
	if (token->type != WORD)
		return (false);
	if (token->value[0] == '\0')
		return (false);
	if (ft_strchr(token->value, '\'') || ft_strchr(token->value, '\"'))
		return (false);
	if (!ft_strchr(token->value, ' ') && !ft_strchr(token->value, '\t'))
		return (false);
	if (token->value[0] == ' '
		|| token->value[ft_strlen(token->value) - 1] == ' ')
		return (false);
	return (true);
}
