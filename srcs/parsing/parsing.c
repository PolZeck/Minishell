/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:20:12 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/22 15:10:35 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_redirections(t_cmd *cmd, t_token **tokens);

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd;
	t_cmd	*head;
	int		arg_count;
	size_t	args_size;

	if (!tokens || tokens->type == PIPE)
	{
		//ft_printf("bash: syntax error near unexpected token `|'\n");
		return (NULL);
	}

	cmd = create_cmd(tokens);
	if (!cmd)
		return (NULL);
	head = cmd;
	arg_count = 0;
	args_size = 2;

	cmd->args = malloc(sizeof(char *) * args_size);
	if (!cmd->args)
		return (free_cmds(head), NULL);

	while (tokens)
	{
		if (tokens->type == PIPE)
		{
			if (!tokens->next || tokens->next->type == PIPE)
			{
				ft_printf("bash: syntax error near unexpected token `|'\n");
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
			handle_argument(cmd, &arg_count, &args_size, tokens->value);
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
