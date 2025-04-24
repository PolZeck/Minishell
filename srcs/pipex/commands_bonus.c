/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:42:11 by lcosson           #+#    #+#             */
/*   Updated: 2025/04/24 15:28:12 by lcosson          ###   ########.fr       */
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

bool	has_output_redir(t_cmd *cmd)
{
	t_list	*node;
	t_redir	*redir;

	if (!cmd)
		return (false);

	node = cmd->redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_OUT || redir->type == APPEND)
			return (true);
		node = node->next;
	}
	return (false);
}

void apply_redirections(t_list *redirs)
{
	t_list *node = redirs;
	t_redir *redir;
	t_redir *last_out = NULL;
	int fd;

	// 1. Appliquer redirections d'entrée (IN / HEREDOC)
	node = redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				perror(redir->file);
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		node = node->next;
	}

	// 2. Ouvrir tous les fichiers OUT pour les effets de bord, sans dup2
	node = redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			int flags = O_WRONLY | O_CREAT | (redir->type == APPEND ? O_APPEND : O_TRUNC);
			fd = open(redir->file, flags, 0644);
			if (fd == -1)
			{
				perror(redir->file);
				exit(1); // quitte immédiatement : ne passe pas à la suite
			}
			close(fd);
			last_out = redir;
		}
		node = node->next;
	}

	// 3. Appliquer dup2 uniquement pour la dernière redirection OUT
	if (last_out)
	{
		int flags = O_WRONLY | O_CREAT | (last_out->type == APPEND ? O_APPEND : O_TRUNC);
		fd = open(last_out->file, flags, 0644);
		if (fd == -1)
		{
			perror(last_out->file);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}


void	execute_cmd(t_cmd *cmd, char **envp, t_pipex *pipex)
{
	char	*path_cmd;

	if (!cmd || !cmd->args || !cmd->args[0])
		exit(1);

	if (is_builtin(cmd->args[0]))
	{
		apply_redirections(cmd->redirs);
		execute_pipex_builtin(cmd->args, envp, pipex);
		clean(pipex);
		close_fds(pipex);
		exit(EXIT_SUCCESS);
	}

	path_cmd = check_addpath_cmd_bonus(cmd->args[0], envp, pipex);
	if (!path_cmd)
	{
		clean(pipex);
		close_fds(pipex);
		exit(127);
	}

	apply_redirections(cmd->redirs);

	if (execve(path_cmd, cmd->args, envp) == -1)
		handle_exec_error(path_cmd, cmd->args);
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

	(void)i;
	(void)argv;

	if (pipe(pipex->pipe_fd) == -1)
		close_and_perr_fork(pipex);

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

		if (!has_output_redir(pipex->current_cmd))
		{
			if (dup2(pipex->pipe_fd[1], STDOUT_FILENO) == -1)
				close_and_perr_dup2(pipex);
		}
		else
			close(pipex->pipe_fd[1]);

		close_fds(pipex);
		execute_cmd(pipex->current_cmd, envp, pipex);
	}

	close(pipex->pipe_fd[1]);
	pipex->prev_pipe_fd = pipex->pipe_fd[0];
	return (pid);
}

pid_t	middle_execution_bonus(int i, char **argv, char **envp, t_pipex *pipex)
{
	pid_t	pid;

	(void)i;
	(void)argv;

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

		if (!has_output_redir(pipex->current_cmd))
		{
			if (dup2(pipex->pipe_fd[1], STDOUT_FILENO) == -1)
				close_and_perr_dup2(pipex);
		}
		else
			close(pipex->pipe_fd[1]);

		clean(pipex);
		close_fds(pipex);
		execute_cmd(pipex->current_cmd, envp, pipex);
	}

	close(pipex->prev_pipe_fd);
	close(pipex->pipe_fd[1]);
	pipex->prev_pipe_fd = pipex->pipe_fd[0];
	return (pid);
}

pid_t	last_execution_bonus(int i, char **argv, char **envp, t_pipex *pipex)
{
	pid_t	pid;

	(void)i;
	(void)argv;

	pid = fork();
	if (pid == -1)
		close_and_perr_fork(pipex);

	if (pid == 0)
	{
		if (!has_input_redir(pipex->current_cmd))
		{
			if (dup2(pipex->prev_pipe_fd, STDIN_FILENO) == -1)
				close_and_perr_dup2(pipex);
		}
		else
			close(pipex->prev_pipe_fd);

		if (!has_output_redir(pipex->current_cmd))
		{
			if (dup2(pipex->out_fd, STDOUT_FILENO) == -1)
			{
				if (pipex->out_fd == -1)
				{
					clean(pipex);
					exit(EXIT_FAILURE);
				}
				close_and_perr_dup2(pipex);
			}
		}
		else
			close(pipex->out_fd);

		clean(pipex);
		close_fds(pipex);
		execute_cmd(pipex->current_cmd, envp, pipex);
	}

	close(pipex->prev_pipe_fd);
	return (pid);
}
