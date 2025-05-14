/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:00:19 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/15 01:29:06 by pol              ###   ########.fr       */
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

char	*expand_line(char *line, t_data *data)
{
	int		i;
	int		var_start;
	char	*result;
	char	*var;
	char	*val;
	char	*tmp;
	char	next[2];

	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1]
			&& (ft_isalnum(line[i + 1]) || line[i + 1] == '_'))
		{
			var_start = ++i;
			while (ft_isalnum(line[i]) || line[i] == '_')
				i++;
			var = ft_substr(line, var_start, i - var_start);
			val = ft_getenv(data, var);
			tmp = ft_strjoin(result, val ? val : "");
			free(result);
			result = tmp;
			free(var);
		}
		else
		{
			next[0] = line[i];
			next[1] = '\0';
			tmp = ft_strjoin(result, next);
			free(result);
			result = tmp;
			i++;
		}
	}
	return (result);
}

static void	write_heredoc_content(int fd, char *delimiter, bool expand, t_data *data)
{
	char	*line;
	char	*expanded;

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
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (expand)
		{
			expanded = expand_line(line, data); // tu la définiras après
			ft_putendl_fd(expanded, fd);
			free(expanded);
		}
		else
			ft_putendl_fd(line, fd);
		free(line);
	}
	setup_signals();
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

	// ✅ juste ici : on déduit si on doit expand ou non la ligne
	expand = ((*tokens)->quote_type != SINGLE_QUOTE);

	filename = NULL;
	fd = open_heredoc_tmp(&filename);
	if (fd < 0)
	{
		perror("open heredoc tmp file");
		free(filename);
		cmd->invalid = 1;
		return ;
	}

	// ✅ on passe expand + data
	write_heredoc_content(fd, (*tokens)->value, expand, data);
	close(fd);

	finalize_heredoc(cmd, filename);
}
