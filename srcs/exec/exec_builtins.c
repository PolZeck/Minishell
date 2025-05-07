/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:30:39 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 10:48:20 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipex_bonus.h"

static int	open_input_redir(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->file);
		*get_exit_status() = 1;
	}
	else
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (fd);
}

static int	open_output_redir(t_redir *redir)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (redir->type == APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(redir->file, flags, 0644);
	if (fd == -1)
	{
		perror(redir->file);
		*get_exit_status() = 1;
	}
	else
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (fd);
}

static int	apply_redirs(t_list *redirs)
{
	t_redir	*redir;
	t_list	*node;
	int		fd;

	node = redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		fd = -1;
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
			fd = open_input_redir(redir);
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
			fd = open_output_redir(redir);
		if (fd == -1)
			return (1);
		node = node->next;
	}
	return (0);
}

static void	run_builtin(t_cmd *cmd, t_data *data)
{
	if (!cmd->args[0])
		return ;
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		*get_exit_status() = builtin_cd(cmd, data);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		*get_exit_status() = builtin_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		*get_exit_status() = builtin_exit(cmd, data);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		*get_exit_status() = builtin_pwd(cmd, data);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		*get_exit_status() = builtin_export(cmd, data);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		*get_exit_status() = builtin_unset(cmd, data);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		*get_exit_status() = builtin_env(cmd, data);
}

void	execute_builtin(t_cmd *cmd, t_data *data)
{
	int	save_stdin;
	int	save_stdout;

	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);
	if (apply_redirs(cmd->redirs))
		return ;
	run_builtin(cmd, data);
	dup2(save_stdin, STDIN_FILENO);
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdin);
	close(save_stdout);
}
