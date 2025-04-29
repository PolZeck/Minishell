/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:20:12 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/29 10:24:13 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_redirections(t_cmd *cmd, t_token **tokens);

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd;
	t_cmd	*head;
	int		arg_count;

	if (!tokens)
		return (NULL);
	if (tokens->type == PIPE)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
		*get_exit_status() = 2;
		return (NULL);
	}
	cmd = create_cmd(tokens);
	if (!cmd)
		return (NULL);
	head = cmd;
	arg_count = 0;
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!tokens->next || tokens->next->type == PIPE)
			{
				ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
				*get_exit_status() = 2;
				free_cmds(head);
				return (NULL);
			}
			tokens = tokens->next;
			cmd->next = create_cmd(tokens);
			if (!cmd->next)
				return (free_cmds(head), NULL);
			cmd = cmd->next;
			arg_count = 0;
			continue ;
		}
		else if (tokens->type == WORD || tokens->type == QUOTE)
		{
			// printf("DEBUG: [%s] (len=%zu)\n", tokens->value, ft_strlen(tokens->value));
			handle_argument(cmd, &arg_count, tokens->value);
		}
		else if (!handle_redirections(cmd, &tokens))
		{
			free_cmds(head);
			return (NULL);
		}
		tokens = tokens->next;
	}
	if (cmd && cmd->args)
		cmd->args[arg_count] = NULL;
	return (head);
}


static int	handle_redirections(t_cmd *cmd, t_token **tokens)
{
	if (!(*tokens)->next || (*tokens)->next->type != WORD)
	{
		ft_printf("Erreur : redirection sans fichier valide\n");
		return (0);
	}
	if ((*tokens)->type == REDIR_IN)
		handle_redir_in(cmd, tokens);
	else if ((*tokens)->type == REDIR_OUT)
		handle_redir_out(cmd, tokens, 0);
	else if ((*tokens)->type == APPEND)
		handle_redir_out(cmd, tokens, 1);
	else if ((*tokens)->type == HEREDOC)
		handle_heredoc(cmd, tokens);
	return (1);
}
