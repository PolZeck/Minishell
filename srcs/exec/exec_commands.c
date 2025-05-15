/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:23:02 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/15 14:05:47 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	exit_clean_pipex(t_pipex *pipex, t_data *data,
		char *cmd_path, int status)
{
	if (cmd_path)
		free(cmd_path);
	if (pipex && pipex->pid)
		free(pipex->pid);
	if (data)
	{
		if (data->cmds_head)
			free_cmds(data->cmds_head);
		if (data->tokens)
			free_tokens(data->tokens);
		if (data->env)
			free_env(data->env);
	}
	exit(status);
}

void	execute_command_and_exit(t_pipex *pipex, t_data *data)
{
	char	*cmd_path;
	int		saved_stdout;
	pid_t	pid;

	if (precheck_command(pipex->current_cmd, data))
		exit_clean_pipex(pipex, data, NULL, *get_exit_status());
	cmd_path = resolve_cmd_path(pipex->current_cmd, data);
	if (!cmd_path)
		exit_clean_pipex(pipex, data, NULL, *get_exit_status());
	enable_ctrl_backslash();
	saved_stdout = dup(STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
		run_child(pipex->current_cmd, data, cmd_path);
	else if (pid > 0)
		wait_and_handle(pid, saved_stdout);
	else
	{
		perror("fork");
		*get_exit_status() = 1;
	}
	exit_clean_pipex(pipex, data, cmd_path, *get_exit_status());
}

void	execute_command(t_cmd *cmd, t_data *data)
{
	char	*cmd_path;
	int		saved_stdout;
	pid_t	pid;

	if (precheck_command(cmd, data))
		return ;
	cmd_path = resolve_cmd_path(cmd, data);
	if (!cmd_path)
		return ;
	enable_ctrl_backslash();
	saved_stdout = dup(STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
		run_child(cmd, data, cmd_path);
	else if (pid > 0)
		wait_and_handle(pid, saved_stdout);
	else
	{
		perror("fork");
		*get_exit_status() = 1;
	}
	free(cmd_path);
}
