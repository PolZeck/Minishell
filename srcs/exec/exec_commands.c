/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:23:02 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 15:34:32 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	precheck_command(t_cmd *cmd, t_data *data)
{
	if (!cmd)
		return (1);
	if (validate_redirections(cmd))
	{
		*get_exit_status() = 1;
		return (1);
	}
	if (!cmd->args || !cmd->args[0])
		return (1);
	if (cmd->args[0][0] == '\0')
	{
		print_error("bash: ", "", ": command not found\n", 127);
		return (1);
	}
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, data);
		return (1);
	}
	return (0);
}

char	*resolve_cmd_path(t_cmd *cmd, t_data *data)
{
	struct stat	st;
	char		*path;

	if (ft_strchr(cmd->args[0], '/'))
	{
		if (stat(cmd->args[0], &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
			{
				print_err("bash: ", cmd->args[0], ": Is a directory\n", 126);
				return (NULL);
			}
			if (access(cmd->args[0], X_OK) != 0)
			{
				print_err("bash: ", cmd->args[0], ": Permission denied\n", 126);
				return (NULL);
			}
			return (ft_strdup(cmd->args[0]));
		}
		print_err("bash: ", cmd->args[0], ": No such file or directory\n", 127);
		return (NULL);
	}
	path = find_command_path(cmd->args[0], data);
	if (!path)
		print_error("bash: ", cmd->args[0], ": command not found\n", 127);
	return (path);
}

void	run_child(t_cmd *cmd, t_data *data, char *path)
{
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
