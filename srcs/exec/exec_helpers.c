/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 09:39:53 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/16 14:32:01 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	check_invalid_command(t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (cmd->args[0][0] == '\0')
	{
		print_err("bash: ", "", ": command not found\n", 127);
		return (1);
	}
	return (0);
}

int	precheck_command(t_cmd *cmd, t_data *data)
{
	if (check_invalid_command(cmd))
		return (1);
	if (validate_redirections(cmd))
	{
		*get_exit_status() = 1;
		return (1);
	}
	if (is_builtin(cmd->args[0]))
	{
		if (ft_strcmp(cmd->args[0], "exit") == 0)
		{
			if (!data->in_pipeline)
				run_builtin(cmd, data);
			return (1);
		}
		execute_builtin(cmd, data);
		return (1);
	}
	return (0);
}

void	run_child(t_cmd *cmd, t_data *data, char *path)
{
	signal(SIGPIPE, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	apply_redirections_in_child(cmd);
	execve(path, cmd->args, data->env);
	perror(path);
	exit(126);
}

void	wait_and_handle(pid_t pid, int saved_stdout)
{
	int	status;
	int	sig;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	disable_ctrl_backslash();
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	if (WIFEXITED(status))
		*get_exit_status() = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		*get_exit_status() = 128 + sig;
		if (sig == SIGINT)
			write(1, "\n", 1);
		else if (sig == SIGQUIT)
			write(1, "Quit (core dumped)\n", 20);
	}
}
