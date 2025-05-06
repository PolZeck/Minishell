/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:20:12 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/06 10:46:16 by pledieu          ###   ########lyon.fr   */
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
		ft_printf("bash: syntax error near unexpected token `|'\n");
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
				ft_printf("bash: syntax error near unexpected token `|'\n");
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
		else if (tokens->type == WORD || tokens->type == QUOTE || tokens->type == DELIMITER)
		{
			// printf("DEBUG token: [%s], quote_type: %d\n", tokens->value, tokens->quote_type);
			// si le token vient d'une expansion, est non-quoté, et contient des espaces → word splitting
			if (tokens->quote_type == NO_QUOTE
				&& ft_strchr(tokens->value, ' ')
				&& tokens->value[0] != '\0')
			{
				char **split = ft_split(tokens->value, ' ');
				for (int i = 0; split && split[i]; i++)
				{
					if (split[i][0] != '\0')
						handle_argument(cmd, &arg_count, split[i]);
				}
				free_split(split);
			}
			else
				handle_argument(cmd, &arg_count, tokens->value);
		}
		else if (!handle_redirections(cmd, &tokens))
		{
			free_cmds(head);
			return (NULL);
		}
		if (cmd->invalid)
		{
			free_cmds(head);
			return (NULL);
		}
		tokens = tokens->next;
	}
	if (cmd && cmd->args)
		cmd->args[arg_count] = NULL;
	t_cmd *cur = head;
	while (cur)
	{
		if (cur->args)
		{
			int i = 0;
			while (cur->args[i])
				i++;
			cur->args[i] = NULL;
		}
		cur = cur->next;
	}
	// t_cmd *tmp = head;
	// while (tmp)
	// {
	// 	if (tmp->args)
	// 	{
	// 		int k = 0;
	// 		while (tmp->args[k])
	// 		{
	// 			printf("ARG[%d] = [%s]\n", k, tmp->args[k]);
	// 			k++;
	// 		}
	// 	}
	// 	tmp = tmp->next;
	// }

	return (head);
}

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

