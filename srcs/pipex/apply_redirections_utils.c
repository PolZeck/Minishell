/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:49:45 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/09 10:53:19 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	open_and_close_output_file(t_redir *redir)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (redir->type == APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(redir->file, flags, 0644);
	if (fd == -1)
	{
		perror(redir->file);
		exit(1);
	}
	close(fd);
}

void	open_and_close_all_outputs(t_list *redirs)
{
	t_list	*node;
	t_redir	*redir;

	node = redirs;
	while (node)
	{
		redir = node->content;
		if (redir->type == REDIR_OUT || redir->type == APPEND)
			open_and_close_output_file(redir);
		node = node->next;
	}
}

t_redir	*find_last_output_redirection(t_list *redirs)
{
	t_list	*node;
	t_redir	*last;
	t_redir	*redir;

	last = NULL;
	node = redirs;
	while (node)
	{
		redir = node->content;
		if (redir->type == REDIR_OUT || redir->type == APPEND)
			last = redir;
		node = node->next;
	}
	return (last);
}
