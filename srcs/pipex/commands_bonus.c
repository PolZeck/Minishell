/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:42:11 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/16 13:15:29 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

bool	has_input_redir(t_cmd *cmd)
{
	t_list	*node;
	t_redir	*redir;

	if (!cmd)
		return (false);
	node = cmd->redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
			return (true);
		node = node->next;
	}
	return (false);
}

bool	has_output_redir(t_cmd *cmd)
{
	t_list	*node;
	t_redir	*redir;

	if (!cmd)
		return (false);
	node = cmd->redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_OUT || redir->type == APPEND)
			return (true);
		node = node->next;
	}
	return (false);
}

void	close_and_clean_in_fd(t_pipex *pipex)
{
	close(pipex->pipe_fd[1]);
	close(pipex->pipe_fd[0]);
	close_fds(pipex);
	clean (pipex);
	exit(1);
}
