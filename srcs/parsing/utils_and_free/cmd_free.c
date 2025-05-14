/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:12:34 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 10:13:48 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	free_cmd_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

static void	free_redir(t_redir *redir)
{
	if (redir->file)
	{
		if (redir->type == HEREDOC)
			unlink(redir->file);
		free(redir->file);
	}
	if (redir->type == HEREDOC && redir->fd != -1)
		close(redir->fd);
	free(redir);
}

static void	free_cmd_redirs(t_list *redirs)
{
	t_list	*next;
	t_redir	*redir;

	while (redirs)
	{
		next = redirs->next;
		redir = (t_redir *)redirs->content;
		if (redir)
			free_redir(redir);
		free(redirs);
		redirs = next;
	}
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_cmd_args(cmd->args);
		free_cmd_redirs(cmd->redirs);
		free(cmd);
		cmd = tmp;
	}
}
