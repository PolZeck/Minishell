/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:10:09 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 09:10:51 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	add_redir_fd(t_cmd *cmd, int type, int fd)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return ;
	redir->type = type;
	redir->file = NULL;
	redir->fd = fd;
	ft_lstadd_back(&cmd->redirs, ft_lstnew(redir));
}

t_redir	*create_redir(int type, char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = file;
	return (redir);
}

void	add_redir(t_cmd *cmd, int type, char *file)
{
	t_redir	*redir;

	redir = create_redir(type, file);
	if (!redir)
		return ;
	ft_lstadd_back(&cmd->redirs, ft_lstnew(redir));
}
