/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:32:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/19 13:42:16 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static bool	validate_first_token(t_token *tokens, t_data *data)
{
	if (!tokens)
		return (false);
	if (tokens->type == PIPE)
	{
		ft_printf("bash: syntax error near unexpected token `|'\n");
		data->exit_status = 2;
		return (false);
	}
	return (true);
}

t_cmd	*parse_tokens(t_token *tokens, t_data *data)
{
	t_cmd	*cmd;
	t_cmd	*head;

	if (!validate_first_token(tokens, data))
		return (NULL);
	cmd = create_cmd(tokens);
	if (!cmd)
		return (NULL);
	head = cmd;
	if (!parse_all_tokens(tokens, cmd, head, data))
		return (NULL);
	finalize_args(head);
	return (head);
}
