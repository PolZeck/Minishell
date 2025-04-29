/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:15:10 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/29 14:09:51 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_argument(t_cmd *cmd, int *arg_count, char *arg)
{
	cmd->args[*arg_count] = ft_strdup(arg);
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
	if ((*tokens)->type != WORD)
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
	if (token->next && token->next->type == WORD)
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

void	handle_heredoc(t_cmd *cmd, t_token **tokens)
{
	int		pipefd[2];
	char	*line;
	char	*delimiter;

	*tokens = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type != WORD)
	{
		ft_printf("Erreur : heredoc sans délimiteur\n");
		cmd->invalid = 1;
		return ;
	}
	delimiter = (*tokens)->value;
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		cmd->invalid = 1;
		return ;
	}
	signal(SIGINT, heredoc_sigint_handler);
	while (1)
	{
		line = readline("> ");
		if (!line) // Ctrl+D ou erreur
			break ;
		if (heredoc_interrupted) // Ctrl+C
		{
			free(line);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, pipefd[1]);
		free(line);
	}
	close(pipefd[1]);
	setup_signals();
	if (heredoc_interrupted)
	{
		close(pipefd[0]);
		cmd->invalid = 1;
		*get_exit_status() = 1;
		heredoc_interrupted = 0;
		return ;
	}
	add_redir_fd(cmd, HEREDOC, pipefd[0]);
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
