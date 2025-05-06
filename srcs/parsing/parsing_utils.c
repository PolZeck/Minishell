/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:15:10 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/06 11:20:01 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_argument(t_cmd *cmd, int *arg_count, char *arg)
{
	int		new_size;
	char	**new_args;

	new_size = (*arg_count) + 2;
	new_args = malloc(sizeof(char *) * new_size);
	if (!new_args)
		return ;

	int i = 0;
	while (i < *arg_count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	(*arg_count)++;
}

void	handle_pipe(t_cmd **cmd, int *arg_count,
			size_t *args_size, t_token **tokens)
{
	(*cmd)->args[*arg_count] = NULL;
	if (!(*tokens)->next || (*tokens)->next->type == PIPE)
	{
		ft_printf("bash: syntax error near unexpected token '|'");
		*tokens = NULL;
		return ;
	}
	*tokens = (*tokens)->next;
	(*cmd)->next = create_cmd(*tokens);
	if (!(*cmd)->next)
		return ;
	(*cmd) = (*cmd)->next;
	*arg_count = 0;
	*args_size = 2;
	(*cmd)->args = malloc(sizeof(char *) * (*args_size));
	if (!(*cmd)->args)
		return ;
}

void	handle_redir_in(t_cmd *cmd, t_token **tokens)
{
	*tokens = (*tokens)->next;
	if (!(*tokens))
	{
		syntax_error("newline");
		cmd->invalid = 1;
		return ;
	}
	if ((*tokens)->type != WORD && (*tokens)->type != DELIMITER)
	{
		if ((*tokens)->type == REDIR_IN || (*tokens)->type == REDIR_OUT
			|| (*tokens)->type == APPEND || (*tokens)->type == HEREDOC
			|| (*tokens)->type == PIPE)
			syntax_error("newline");
		else
			syntax_error((*tokens)->value);
		cmd->invalid = 1;
		return ;
	}
	add_redir(cmd, REDIR_IN, ft_strdup((*tokens)->value));
}

void	handle_redir_out(t_cmd *cmd, t_token **tokens, int append)
{
	t_token	*token;

	token = *tokens;
	if (token->next && (token->next->type == WORD || token->next->type == DELIMITER))
	{
		if (append)
			add_redir(cmd, APPEND, ft_strdup(token->next->value));
		else
			add_redir(cmd, REDIR_OUT, ft_strdup(token->next->value));
		*tokens = token->next;
	}
	else
	{
		if (!token->next)
			syntax_error("newline");
		else
			syntax_error(token->next->value);
		cmd->invalid = 1;
	}
}

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

void generate_random_name(char *output, size_t len)
{
	const char charset[] = "abcdefghijklmnopqrstuvwxyz0123456789";
	unsigned char buf[12];
	int fd;
	size_t i;

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
	if ((*tokens)->type != WORD && (*tokens)->type != QUOTE && (*tokens)->type != DELIMITER)
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
		if (heredoc_interrupted)
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

	if (heredoc_interrupted)
	{
		unlink(filename);
		free(filename);
		cmd->invalid = 1;
		*get_exit_status() = 1;
		heredoc_interrupted = 0;
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

void	syntax_error(char *token)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
	*get_exit_status() = 2;
}
