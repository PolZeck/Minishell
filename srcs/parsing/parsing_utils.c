/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:15:10 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/12 16:10:32 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_argument(t_cmd *cmd, int *arg_count,
		size_t *args_size, char *value)
{
	if (*arg_count + 1 >= (int)*args_size)
	{
		*args_size *= 2;
		cmd->args = ft_realloc(cmd->args, sizeof(char *) * (*arg_count + 1),
				sizeof(char *) * (*args_size));
		if (!cmd->args)
			return ;
	}
	cmd->args[*arg_count] = ft_strdup(value);
	(*arg_count)++;
}

void	handle_pipe(t_cmd **cmd, int *arg_count, size_t *args_size, t_token **tokens)
{
	(*cmd)->args[*arg_count] = NULL;
	if (!(*tokens)->next || (*tokens)->next->type == PIPE)
	{
		perror("Erreur de syntaxe : pipe sans commande après");
		*tokens = NULL;
		return ;
	}
	*tokens = (*tokens)->next;
	(*cmd)->next = create_cmd();
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
		return ;
	}
	if ((*tokens)->type != WORD)
	{
		ft_printf("Erreur : fichier invalide pour la redirection\n");
		return ;
	}
	cmd->infile = ft_strdup((*tokens)->value);
}

void	handle_redir_out(t_cmd *cmd, t_token **tokens, int append)
{
	*tokens = (*tokens)->next;
	if (!(*tokens))
	{
		ft_printf("Erreur : redirection sans fichier\n");
		return ;
	}
	if ((*tokens)->type != WORD)
	{
		ft_printf("Erreur : fichier invalide pour la redirection\n");
		return ;
	}
	cmd->outfile = ft_strdup((*tokens)->value);
	cmd->append = append;
	if (append)
		ft_printf("Écriture en mode APPEND vers : %s\n", cmd->outfile);
}

void	handle_heredoc(t_cmd *cmd, t_token **tokens)
{
	*tokens = (*tokens)->next;
	if (!(*tokens))
	{
		ft_printf("Erreur : heredoc sans délimiteur\n");
		return ;
	}
	if ((*tokens)->type != WORD)
	{
		ft_printf("Erreur : délimiteur invalide pour le heredoc\n");
		return ;
	}
	cmd->infile = ft_strdup((*tokens)->value);
	ft_printf("Lecture depuis un HEREDOC avec délimiteur : %s\n", cmd->infile);
}
