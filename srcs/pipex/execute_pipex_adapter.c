/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipex_adapter.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:50:45 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/09 09:10:22 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_cmds(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

static pid_t	first_execution_direct(t_pipex *pipex, t_data *data)
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

static pid_t	middle_execution_direct(t_pipex *pipex, t_data *data)
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

static pid_t	last_execution_direct(t_pipex *pipex, t_data *data)
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

int	execute_pipex_direct(t_cmd *cmds, t_data *data)
{
	t_pipex	pipex;
	t_cmd	*current;
	int		i;
	int		exit_code;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	ft_memset(&pipex, 0, sizeof(t_pipex));
	pipex.in_fd = STDIN_FILENO;
	pipex.out_fd = STDOUT_FILENO;
	pipex.num_cmds = count_cmds(cmds);
	pipex.pid = malloc(sizeof(pid_t) * pipex.num_cmds);
	if (!pipex.pid)
		return (1);
	current = cmds;
	i = 0;
	while (current)
	{
		pipex.current_cmd = current;
		if (i == 0)
			pipex.pid[i] = first_execution_direct(&pipex, data);
		else if (i == pipex.num_cmds - 1)
			pipex.pid[i] = last_execution_direct(&pipex, data);
		else
			pipex.pid[i] = middle_execution_direct(&pipex, data);
		current = current->next;
		i++;
	}
	close_fds(&pipex);
	exit_code = wait_for_processes(&pipex);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	return (exit_code);
}
