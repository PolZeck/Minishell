/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:00:19 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 09:03:51 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	generate_random_name(char *output, size_t len)
{
	const char		charset[] = "abcdefghijklmnopqrstuvwxyz0123456789";
	unsigned char	buf[12];
	int				fd;
	size_t			i;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
	{
		write(2, "Erreur ouverture /dev/urandom\n", 31);
		exit(1);
	}
	if (read(fd, buf, len) != (ssize_t)len)
	{
		write(2, "Erreur lecture /dev/urandom\n", 29);
		close(fd);
		exit(1);
	}
	close(fd);
	i = 0;
	while (i < len)
	{
		output[i] = charset[buf[i] % (sizeof(charset) - 1)];
		i++;
	}
	output[len] = '\0';
}

void	handle_heredoc(t_cmd *cmd, t_token **tokens)
{
	char		randname[13];
	char		*tmp_path;
	char		*filename;
	char		*line;
	char		*delimiter;
	t_redir		*redir;
	int			fd;

	*tokens = (*tokens)->next;
	if (!(*tokens))
	{
		syntax_error("newline");
		cmd->invalid = 1;
		return ;
	}
	if ((*tokens)->type != WORD && (*tokens)->type != QUOTE
		&& (*tokens)->type != DELIMITER)
	{
		syntax_error((*tokens)->value);
		cmd->invalid = 1;
		return ;
	}
	(*tokens)->type = DELIMITER;
	delimiter = (*tokens)->value;
	generate_random_name(randname, 12);
	tmp_path = ft_strjoin("/tmp/", "heredoc_");
	if (!tmp_path)
		return ;
	filename = ft_strjoin(tmp_path, randname);
	free(tmp_path);
	if (!filename)
		return ;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		perror("open heredoc tmp file");
		free(filename);
		cmd->invalid = 1;
		return ;
	}
	signal(SIGINT, heredoc_sigint_handler);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (g_heredoc_interrupted)
		{
			free(line);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	setup_signals();
	if (g_heredoc_interrupted)
	{
		unlink(filename);
		free(filename);
		cmd->invalid = 1;
		*get_exit_status() = 1;
		g_heredoc_interrupted = 0;
		return ;
	}
	redir = malloc(sizeof(t_redir));
	if (!redir)
	{
		free(filename);
		return ;
	}
	redir->type = HEREDOC;
	redir->file = filename;
	redir->fd = open(filename, O_RDONLY);
	if (redir->fd == -1)
	{
		perror("open heredoc read");
		free(redir->file);
		free(redir);
		cmd->invalid = 1;
		return ;
	}
	ft_lstadd_back(&cmd->redirs, ft_lstnew(redir));
}
