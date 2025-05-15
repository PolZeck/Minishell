/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions_direct.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:14:29 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/15 17:28:38 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

pid_t	first_execution_direct(t_pipex *pipex, t_data *data)
{
	pid_t	pid;

	if (pipe(pipex->pipe_fd) == -1)
		my_perr(ERR_PIPE, true);
	pid = fork();
	if (pid == -1)
		my_perr(ERR_FORK, true);
	if (pid == 0)
	{
		close(pipex->pipe_fd[0]);
		if (pipex->in_fd != -1 && !has_input_redir(pipex->current_cmd))
			dup2(pipex->in_fd, STDIN_FILENO);
		else if (pipex->in_fd == -1 && !has_input_redir(pipex->current_cmd))
			exit(1);
		if (!has_output_redir(pipex->current_cmd))
			dup2(pipex->pipe_fd[1], STDOUT_FILENO);
		apply_redirections(pipex->current_cmd->redirs);
		close_fds(pipex);
		execute_command_and_exit(pipex, data);
	}
	close(pipex->pipe_fd[1]);
	pipex->prev_pipe_fd = pipex->pipe_fd[0];
	return (pid);
}

pid_t	middle_execution_direct(t_pipex *pipex, t_data *data)
{
	pid_t	pid;

	if (pipe(pipex->pipe_fd) == -1)
		my_perr(ERR_PIPE, true);
	pid = fork();
	if (pid == -1)
		my_perr(ERR_FORK, true);
	if (pid == 0)
	{
		close(pipex->pipe_fd[0]);
		if (!has_input_redir(pipex->current_cmd))
			dup2(pipex->prev_pipe_fd, STDIN_FILENO);
		if (!has_output_redir(pipex->current_cmd))
			dup2(pipex->pipe_fd[1], STDOUT_FILENO);
		apply_redirections(pipex->current_cmd->redirs);
		close_fds(pipex);
		execute_command_and_exit(pipex, data);
	}
	close(pipex->prev_pipe_fd);
	close(pipex->pipe_fd[1]);
	pipex->prev_pipe_fd = pipex->pipe_fd[0];
	return (pid);
}

pid_t	last_execution_direct(t_pipex *pipex, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		my_perr(ERR_FORK, true);
	if (pid == 0)
	{
		if (!has_input_redir(pipex->current_cmd))
			dup2(pipex->prev_pipe_fd, STDIN_FILENO);
		if (pipex->out_fd != -1 && !has_output_redir(pipex->current_cmd))
			dup2(pipex->out_fd, STDOUT_FILENO);
		apply_redirections(pipex->current_cmd->redirs);
		close_fds(pipex);
		execute_command_and_exit(pipex, data);
	}
	close(pipex->prev_pipe_fd);
	return (pid);
}
