/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:23:47 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/09 10:50:23 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	apply_input_redirections(t_list *redirs)
{
	t_list	*node;
	t_redir	*redir;
	int		fd;

	node = redirs;
	while (node)
	{
		redir = node->content;
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				perror(redir->file);
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		node = node->next;
	}
}

static void	apply_output_redirection(t_redir *redir)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (!redir)
		return ;
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
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	apply_redirections(t_list *redirs)
{
	t_redir	*last_out;

	apply_input_redirections(redirs);
	open_and_close_all_outputs(redirs);
	last_out = find_last_output_redirection(redirs);
	apply_output_redirection(last_out);
}
