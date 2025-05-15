/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:26:15 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/15 12:33:19 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	write_and_free(char *line, int fd)
{
	ft_putendl_fd(line, fd);
	free(line);
}

static void	write_expanded_line(char *line, int fd, t_data *data)
{
	char	*expanded;

	expanded = expand_line(line, data);
	ft_putendl_fd(expanded, fd);
	free(expanded);
}

static bool	is_delimiter(const char *line, const char *delimiter)
{
	if (ft_strcmp((char *)line, (char *)delimiter) == 0)
		return (true);
	return (false);
}

void	write_heredoc_content(int fd,
		char *delimiter, bool expand, t_data *data)
{
	char	*line;

	signal(SIGINT, heredoc_sigint_handler);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_heredoc_interrupted)
				open("/dev/tty", O_RDONLY);
			break ;
		}
		if (is_delimiter(line, delimiter))
		{
			free(line);
			break ;
		}
		if (expand)
			write_expanded_line(line, fd, data);
		else
			write_and_free(line, fd);
	}
	setup_signals();
}
