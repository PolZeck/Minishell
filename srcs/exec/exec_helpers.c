/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 09:39:53 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/19 14:08:46 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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
		print_err("bash: ", "", ": command not found\n", 127);
		return (1);
	}
	if (is_builtin(cmd->args[0]))
	{
		if (run_exit_exception(cmd, data) == 1)
			return (1);
		execute_builtin(cmd, data);
		return (1);
	}
	return (0);
}

static char	*check_absolute_path(t_cmd *cmd)
{
	struct stat	st;

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

char	*resolve_cmd_path(t_cmd *cmd, t_data *data)
{
	char	*path;
	char	*env_path;
	char	*fallback;

	if (ft_strchr(cmd->args[0], '/'))
		return (check_absolute_path(cmd));
	env_path = ft_getenv(data, "PATH");
	if (!env_path || env_path[0] == '\0')
	{
		fallback = ft_strjoin("./", cmd->args[0]);
		if (access(fallback, X_OK) == 0)
			return (fallback);
		free(fallback);
		print_err("bash: ", cmd->args[0], ": No such file or directory\n", 127);
		return (NULL);
	}
	path = find_command_path(cmd->args[0], data);
	if (!path)
		print_err("bash: ", cmd->args[0], ": command not found\n", 127);
	return (path);
}

void    close_fds_except_std(void)
{
    int    fd;

    fd = 3;
    while (fd < 1024)
    {
        close(fd);
        fd++;
    }
}

void    run_child(t_cmd *cmd, t_data *data, char *path)
{
	signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    apply_redirections_in_child(cmd);
    close_fds_except_std();
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
		if (sig == SIGINT)
			write(1, "\n", 1);
		else if (sig == SIGQUIT)
			write(1, "Quit (core dumped)\n", 20);
		if (sig != SIGPIPE)
			*get_exit_status() = 128 + sig;
		else
			*get_exit_status() = 0;
	}
}
