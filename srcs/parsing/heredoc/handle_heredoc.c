/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:00:19 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/15 16:20:00 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	check_heredoc_syntax(t_cmd *cmd, t_token **tokens)
{
	*tokens = (*tokens)->next;
	if (!(*tokens))
	{
		syntax_error("newline");
		cmd->invalid = 1;
		return (0);
	}
	if ((*tokens)->type != WORD && (*tokens)->type
		!= QUOTE && (*tokens)->type != DELIMITER)
	{
		syntax_error((*tokens)->value);
		cmd->invalid = 1;
		return (0);
	}
	(*tokens)->type = DELIMITER;
	return (1);
}

static int	open_heredoc_tmp(char **filename)
{
	char	randname[13];
	char	*tmp;

	generate_random_name(randname, 12);
	tmp = ft_strjoin("/tmp/", "heredoc_");
	if (!tmp)
		return (-1);
	*filename = ft_strjoin(tmp, randname);
	free(tmp);
	if (!(*filename))
		return (-1);
	return (open(*filename, O_WRONLY | O_CREAT | O_TRUNC, 0600));
}

static int	finalize_heredoc(t_cmd *cmd, char *filename)
{
	t_redir	*redir;

	if (if_g_heredoc_interrupted(cmd, filename))
		return (0);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (free(filename), 0);
	redir->type = HEREDOC;
	redir->file = filename;
	redir->fd = open(filename, O_RDONLY);
	if (redir->fd == -1)
	{
		perror("open heredoc read");
		free(redir->file);
		free(redir);
		cmd->invalid = 1;
		return (0);
	}
	ft_lstadd_back(&cmd->redirs, ft_lstnew(redir));
	return (1);
}

void	handle_heredoc(t_cmd *cmd, t_token **tokens, t_data	*data)
{
	char	*filename;
	bool	expand;
	int		fd;

	if (g_heredoc_interrupted)
	{
		cmd->invalid = 1;
		return ;
	}
	if (!check_heredoc_syntax(cmd, tokens))
		return ;
	expand = ((*tokens)->quote_type == NO_QUOTE);
	filename = NULL;
	fd = open_heredoc_tmp(&filename);
	if (fd < 0)
	{
		perror("open heredoc tmp file");
		free(filename);
		cmd->invalid = 1;
		return ;
	}
	write_heredoc_content(fd, (*tokens)->value, expand, data);
	close(fd);
	finalize_heredoc(cmd, filename);
}
