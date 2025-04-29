/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:30:39 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/29 16:45:24 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipex_bonus.h"

void	execute_builtin(t_cmd *cmd, t_data *data)
{
	int		save_stdin;
	int		save_stdout;
	int		fd;
	int		flags;
	t_list	*node;
	t_redir	*redir;

	save_stdout = -1;
	save_stdin = -1;
	node = cmd->redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				perror(redir->file);
				*get_exit_status() = 1;
				exit(EXIT_FAILURE);
			}
			if (save_stdin != -1)
				close(save_stdin);
			save_stdin = dup(STDIN_FILENO);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
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
				return ;
			}
			if (save_stdout != -1)
				close(save_stdout);
			save_stdout = dup(STDOUT_FILENO);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		node = node->next;
	}
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
	if (save_stdin != -1)
	{
		dup2(save_stdin, STDIN_FILENO);
		close(save_stdin);
	}
	if (save_stdout != -1)
	{
		dup2(save_stdout, STDOUT_FILENO);
		close(save_stdout);
	}
}

void	execute_pipex_builtin(char **args, char **envp, t_pipex *pipex)
{
	t_cmd	cmd;
	t_data	data;

	cmd.args = args;
	cmd.redirs = NULL;
	cmd.invalid = 0;
	cmd.next = NULL;
	data.env = envp;
	execute_builtin(&cmd, &data);
	clean(pipex);
	close_fds(pipex);
	exit(*get_exit_status());
}
