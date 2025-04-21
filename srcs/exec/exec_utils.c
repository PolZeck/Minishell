/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:21:33 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/21 13:21:59 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	handle_redi(t_list *redirs)
{
	t_redir	*r;
	int		fd;

	while (redirs)
	{
		r = (t_redir *)redirs->content;
		fd = -1;
		if (r->type == R_IN)
			fd = open(r->filename, O_RDONLY);
		else if (r->type == R_OUT)
			fd = open(r->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (r->type == R_APPEND)
			fd = open(r->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			print_error("bash: ", r->filename, ": ");
			perror("");
			return (-1);
		}
		dup2(fd, (r->type == R_IN) ? STDIN_FILENO : STDOUT_FILENO);
		close(fd);
		redirs = redirs->next;
	}
	return (0);
}
