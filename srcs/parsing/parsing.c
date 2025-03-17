/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:20:12 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/17 16:12:46 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd;
	t_cmd	*head;
	int		arg_count;
	size_t	args_size;

	if (!tokens || tokens->type == PIPE)
		return (NULL);
	cmd = create_cmd();
	head = cmd;
	arg_count = 0;
	args_size = 2;
	cmd->args = malloc(sizeof(char *) * args_size);
	if (!cmd->args)
		return (NULL);
	while (tokens)
	{
		if (tokens->type == PIPE)
		{
    		if (!tokens->next || tokens->next->type == PIPE)
			{
				perror("Erreur de syntaxe : pipe mal placé");
				if(head)
					free_cmds(head);
				return (NULL);
			}
			handle_pipe(&cmd, &arg_count, &args_size, &tokens);
			continue;	
		}
		else if (tokens->type == WORD || tokens->type == QUOTE)
		{
			handle_argument(cmd, &arg_count, &args_size, tokens->value);
			if (arg_count == 1 && !is_builtin(cmd->args[0]))
    			mark_invalid(cmd, "Commande non supportée");
		}
		else
			handle_redirections(cmd, &tokens);
		tokens = tokens->next;
	}
	cmd->args[arg_count] = NULL;
	return (head);
}

void	handle_redirections(t_cmd *cmd, t_token **tokens)
{
    if (!(*tokens)->next || (*tokens)->next->type != WORD)
    {
        ft_printf("Erreur : redirection sans fichier valide\n");
        free_cmds(cmd);
        *tokens = NULL;
        return;
    }
    if ((*tokens)->type == REDIR_IN)
        handle_redir_in(cmd, tokens);
    else if ((*tokens)->type == REDIR_OUT)
        handle_redir_out(cmd, tokens, 0);
    else if ((*tokens)->type == APPEND)
        handle_redir_out(cmd, tokens, 1);
    else if ((*tokens)->type == HEREDOC)
        handle_heredoc(cmd, tokens);
}

