/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:15:10 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/23 16:50:55 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_argument(t_cmd *cmd, int *arg_count, char *arg)
{
	cmd->args[*arg_count] = ft_strdup(arg);
	(*arg_count)++;
}

void	handle_pipe(t_cmd **cmd, int *arg_count,
			size_t *args_size, t_token **tokens)
{
	(*cmd)->args[*arg_count] = NULL;
	if (!(*tokens)->next || (*tokens)->next->type == PIPE)
	{
		ft_printf("bash: syntax error near unexpected token '|'");
		*tokens = NULL;
		return ;
	}
	*tokens = (*tokens)->next;
	(*cmd)->next = create_cmd(*tokens);
	if (!(*cmd)->next)
		return ;
	(*cmd) = (*cmd)->next;
	*arg_count = 0;
	*args_size = 2;
	(*cmd)->args = malloc(sizeof(char *) * (*args_size));
	if (!(*cmd)->args)
		return ;
}

void	handle_redir_in(t_cmd *cmd, t_token **tokens)
{
	*tokens = (*tokens)->next;
	if (!(*tokens))
	{
		ft_printf("Erreur : redirection sans fichier\n");
		cmd->invalid = 1;
		return ;
	}
	if ((*tokens)->type != WORD)
	{
		ft_printf("Erreur : fichier invalide pour la redirection\n");
		cmd->invalid = 1;
		return ;
	}
	add_redir(cmd, REDIR_IN, ft_strdup((*tokens)->value));
}


void	handle_redir_out(t_cmd *cmd, t_token **tokens, int append)
{
	t_token *token = *tokens;

	if (token->next && token->next->type == WORD)
	{
		add_redir(cmd, append ? APPEND : REDIR_OUT, ft_strdup(token->next->value));
		*tokens = token->next;
	}
	else
	{
		ft_printf("Erreur : redirection de sortie sans fichier\n");
		cmd->invalid = 1;
	}
}


void	handle_heredoc(t_cmd *cmd, t_token **tokens)
{
	*tokens = (*tokens)->next;
	if (!(*tokens))
	{
		ft_printf("Erreur : heredoc sans délimiteur\n");
		cmd->invalid = 1;
		return ;
	}
	if ((*tokens)->type != WORD)
	{
		ft_printf("Erreur : délimiteur invalide pour le heredoc\n");
		cmd->invalid = 1;
		return ;
	}
	add_redir(cmd, HEREDOC, ft_strdup((*tokens)->value));
	ft_printf("Lecture depuis un HEREDOC avec délimiteur : %s\n", (*tokens)->value);
}



t_redir	*create_redir(int type, char *file)
{
	t_redir *redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = file; // 🔥 tu peux faire un strdup ici si besoin
	return (redir);
}

void	add_redir(t_cmd *cmd, int type, char *file)
{
	t_redir *redir = create_redir(type, file);
	if (!redir)
		return ; // ou gérer erreur mémoire
	ft_lstadd_back(&cmd->redirs, ft_lstnew(redir));
}
