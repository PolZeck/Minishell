/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:28:33 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/15 17:13:51 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	open_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		perror(filename);
	return (fd);
}

int	open_output(char *filename, int type)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (type == APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
		perror(filename);
	return (fd);
}

int	validate_output_redir(t_redir *redir)
{
	int	fd;
	int	flags;

	if (!redir->file)
	{
		ft_putstr_fd("bash: redirection file missing\n", 2);
		return (1);
	}
	flags = O_WRONLY | O_CREAT;
	if (redir->type == APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(redir->file, flags, 0644);
	if (fd == -1)
	{
		perror(redir->file);
		return (1);
	}
	close(fd);
	return (0);
}

int	validate_input_redir(t_redir *redir)
{
	int	fd;

	if (!redir->file && redir->type != HEREDOC)
	{
		ft_putstr_fd("bash: redirection file missing\n", 2);
		return (1);
	}
	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("bash: ", STDERR_FILENO);
		perror(redir->file);
		return (1);
	}
	close(fd);
	return (0);
}

int	validate_redirections(t_cmd *cmd)
{
	t_list	*node;
	t_redir	*redir;

	node = cmd->redirs;
	while (node)
	{
		redir = node->content;
		if (redir->type == REDIR_IN)
		{
			if (validate_input_redir(redir))
				return (1);
		}
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			if (validate_output_redir(redir))
				return (1);
		}
		node = node->next;
	}
	return (0);
}
