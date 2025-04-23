/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:42:11 by lcosson           #+#    #+#             */
/*   Updated: 2025/04/23 13:22:03 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

bool	has_input_redir(t_cmd *cmd)
{
	t_list	*node;
	t_redir	*redir;

	if (!cmd)
		return false;
	node = cmd->redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
			return true;
		node = node->next;
	}
	return false;
}

void	execute_cmd(char *cmd, char **envp, t_pipex *pipex)
{
	char	**tokenized_cmd;
	char	*path_cmd;

	tokenized_cmd = ft_split(cmd, ' ');
	if (!tokenized_cmd)
	{
		close_fds(pipex);
		my_perr(ERR_SPLIT, true);
	}

	if (is_builtin(tokenized_cmd[0]))
		execute_pipex_builtin(tokenized_cmd, envp, pipex);

	path_cmd = check_addpath_cmd_bonus(tokenized_cmd[0], envp, pipex);
	if (!path_cmd)
	{
		clean (pipex);
		my_free2((void **)tokenized_cmd);
		close_fds(pipex);
		exit(127);
	}
	
	t_list *node = pipex->current_cmd->redirs;
	t_redir *redir;
	int fd;

	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				perror(redir->file);
				close_fds(pipex);
				clean(pipex);
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			int flags = O_WRONLY | O_CREAT | (redir->type == APPEND ? O_APPEND : O_TRUNC);
			fd = open(redir->file, flags, 0644);
			if (fd == -1)
			{
				perror(redir->file);
				close_fds(pipex);
				clean(pipex);
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		node = node->next;
	}
	if (execve(path_cmd, tokenized_cmd, envp) == -1)
		handle_exec_error(path_cmd, tokenized_cmd);
}

void	close_and_clean_in_fd(t_pipex *pipex)
{
	close(pipex->pipe_fd[1]);
	close_fds(pipex);
	clean (pipex);
	exit(1);
}

pid_t	first_execution_bonus(int i, char *argv[], char *envp[], t_pipex *pipex)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		close_and_perr_fork(pipex);
	if (pid == 0)
	{
		close(pipex->pipe_fd[0]);
		if (pipex->in_fd == -1)
			close_and_clean_in_fd(pipex);
		if (dup2(pipex->in_fd, STDIN_FILENO) == -1)
			close_and_perr_dup2(pipex);
		close(pipex->in_fd);
		if (dup2(pipex->pipe_fd[1], STDOUT_FILENO) == -1)
			close_and_perr_dup2(pipex);
		close_fds(pipex);
		execute_cmd(argv[i + 2 + pipex->here_doc_flag], envp, pipex);
	}
	close(pipex->pipe_fd[1]);
	pipex->prev_pipe_fd = pipex->pipe_fd[0];
	return (pid);
}

pid_t	last_execution_bonus(int i, char **argv, char **envp, t_pipex *pipex)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		close_and_perr_fork(pipex);
	if (pid == 0)
	{
		close(pipex->pipe_fd[1]);
		if (!has_input_redir(pipex->current_cmd))
		{
			if (dup2(pipex->prev_pipe_fd, STDIN_FILENO) == -1)
				close_and_perr_dup2(pipex);
		}
		else
			close(pipex->prev_pipe_fd);

		if (dup2(pipex->out_fd, STDOUT_FILENO) == -1)
		{
			if (pipex->out_fd == -1)
			{
				clean(pipex);
				exit(EXIT_FAILURE);
			}
			close_and_perr_dup2(pipex);
		}
		clean(pipex);
		close_fds(pipex);
		execute_cmd(argv[i + 2 + pipex->here_doc_flag], envp, pipex);
	}
	close(pipex->prev_pipe_fd);
	return (pid);
}


pid_t	middle_execution_bonus(int i, char **argv, char **envp, t_pipex *pipex)
{
	pid_t	pid;

	if (pipe(pipex->pipe_fd) == -1)
		close_and_perr_fork(pipex);
	pid = fork();
	if (pid == -1)
		close_and_perr_fork(pipex);
	if (pid == 0)
	{
		close(pipex->pipe_fd[0]);
		if (!has_input_redir(pipex->current_cmd))
		{
			if (dup2(pipex->prev_pipe_fd, STDIN_FILENO) == -1)
				close_and_perr_dup2(pipex);
		}
		else
			close(pipex->prev_pipe_fd);

		if (dup2(pipex->pipe_fd[1], STDOUT_FILENO) == -1)
			close_and_perr_dup2(pipex);
		clean(pipex);
		close_fds(pipex);
		execute_cmd(argv[i + 2 + pipex->here_doc_flag], envp, pipex);
	}
	close(pipex->prev_pipe_fd);
	close(pipex->pipe_fd[1]);
	pipex->prev_pipe_fd = pipex->pipe_fd[0];
	return (pid);
}

