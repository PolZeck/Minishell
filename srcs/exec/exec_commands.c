/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:23:02 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/29 10:50:48 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command_path(char *cmd)
{
	char	*path;
	char	**paths;
	char	*prefix;
	char	*full_path;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
	path = getenv("PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		prefix = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(prefix, cmd);
		free(prefix);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}


void print_error(char *prefix, char *cmd, char *message)
{
    ft_putstr_fd(prefix, STDERR_FILENO);
    ft_putstr_fd(cmd, STDERR_FILENO);
    ft_putstr_fd(message, STDERR_FILENO);
}

int	validate_redirections(t_cmd *cmd)
{
	t_list	*node = cmd->redirs;
	t_redir	*redir;
	int		fd;

	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
			fd = open(redir->file, O_RDONLY);
		else
			fd = open(redir->file,
				O_WRONLY | O_CREAT | (redir->type == APPEND ? O_APPEND : O_TRUNC),
				0644);
		if (fd == -1)
		{
			perror(redir->file);
			return (1);
		}
		close(fd);
		node = node->next;
	}
	return (0);
}

void	execute_command(t_cmd *cmd, t_data *data)
{
	pid_t		pid;
	char		*cmd_path;
	struct stat	st;

	if (!cmd)
		return ;
	if (validate_redirections(cmd))
	{
		*get_exit_status() = 1;
		return ;
	}
	if (!cmd->args || !cmd->args[0])
		return ;
	if (cmd->args[0][0] == '\0')
	{
		print_error("bash: ", "", ": command not found\n");
		*get_exit_status() = 127;
		return ;
	}
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, data);
		return ;
	}
	if (ft_strchr(cmd->args[0], '/'))
	{
		if (stat(cmd->args[0], &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
			{
				print_error("bash: ", cmd->args[0], ": Is a directory\n");
				*get_exit_status() = 126;
				return ;
			}
			if (access(cmd->args[0], X_OK) != 0)
			{
				print_error("bash: ", cmd->args[0], ": Permission denied\n");
				*get_exit_status() = 126;
				return ;
			}
			cmd_path = ft_strdup(cmd->args[0]);
		}
		else
		{
			print_error("bash: ", cmd->args[0], ": No such file or directory\n");
			*get_exit_status() = 127;
			return ;
		}
	}
	else
	{
		cmd_path = find_command_path(cmd->args[0]);
		if (!cmd_path)
		{
			print_error("bash: ", cmd->args[0], ": command not found\n");
			*get_exit_status() = 127;
			return ;
		}
	}
	enable_ctrl_backslash();
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		t_list *node = cmd->redirs;
		while (node)
		{
			t_redir *redir = (t_redir *)node->content;
			int fd;

			if (redir->type == REDIR_IN || redir->type == HEREDOC)
			{
				fd = open(redir->file, O_RDONLY);
				if (fd == -1)
				{
					print_error("bash: ", redir->file, ": ");
					perror("");
					exit(1);
				}
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
			else if (redir->type == REDIR_OUT || redir->type == APPEND)
			{
				int flags = O_WRONLY | O_CREAT;
				flags |= (redir->type == APPEND) ? O_APPEND : O_TRUNC;
				fd = open(redir->file, flags, 0644);
				if (fd == -1)
				{
					print_error("bash: ", redir->file, ": ");
					perror("");
					exit(1);
				}
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
			node = node->next;
		}

		if (execve(cmd_path, cmd->args, data->env) == -1)
		{
			print_error("bash: ", cmd_path, ": ");
			perror("");
			exit(126);
		}
	}
	else if (pid > 0)
	{
		int status;
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		disable_ctrl_backslash();
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, sigquit_handler);
		if (WIFEXITED(status))
			*get_exit_status() = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			int sig = WTERMSIG(status);
			*get_exit_status() = 128 + sig;
			if (sig == SIGINT)
				write(1, "\n", 1);
			else if (sig == SIGQUIT)
				write(1, "Quit (core dumped)\n", 20);
		}
	}
	else
	{
		perror("fork");
		*get_exit_status() = 1;
	}
	free(cmd_path);
}
