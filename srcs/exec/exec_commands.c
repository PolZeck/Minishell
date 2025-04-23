/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:23:02 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/23 13:05:42 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*find_command_path(char *cmd)
{
	char	*path;
	char	**paths;
	char	*full_path;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
	path = getenv("PATH"); // Récupérer la variable PATH
	if (!path)
		return (NULL);
	paths = ft_split(path, ':'); // Diviser les chemins par ':'
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, cmd);
		if (access(full_path, X_OK) == 0)			
		{
			while (paths[i]) // Libérer `split()`
				free(paths[i++]);
			free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	i = 0;
	while (paths[i]) // Libérer `split()`
		free(paths[i++]);
	free(paths);
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
	pid_t	pid;
	char	*cmd_path;
	struct stat	st;

	if (!cmd || !cmd->args || !cmd->args[0])
		return;

	if (validate_redirections(cmd))
	{
		*get_exit_status() = 1;
		return;
	}

	if (cmd->args[0][0] == '\0')
	{
		print_error("bash: ", "", ": command not found\n");
		*get_exit_status() = 127;
		return;
	}

	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, data);
		return;
	}

	if (ft_strchr(cmd->args[0], '/'))
	{
		if (stat(cmd->args[0], &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
			{
				print_error("bash: ", cmd->args[0], ": Is a directory\n");
				*get_exit_status() = 126;
				return;
			}
			if (access(cmd->args[0], X_OK) != 0)
			{
				print_error("bash: ", cmd->args[0], ": Permission denied\n");
				*get_exit_status() = 126;
				return;
			}
			cmd_path = ft_strdup(cmd->args[0]);
		}
		else
		{
			print_error("bash: ", cmd->args[0], ": No such file or directory\n");
			*get_exit_status() = 127;
			return;
		}
	}
	else
	{
		cmd_path = find_command_path(cmd->args[0]);
		if (!cmd_path)
		{
			print_error("bash: ", cmd->args[0], ": command not found\n");
			*get_exit_status() = 127;
			return;
		}
	}
	
	pid = fork();
	if (pid == 0)
	{
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
				if (redir->type == APPEND)
					flags |= O_APPEND;
				else
					flags |= O_TRUNC;
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
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			*get_exit_status() = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*get_exit_status() = 128 + WTERMSIG(status);
	}
	else
	{
		perror("fork");
		*get_exit_status() = 1;
	}

	free(cmd_path);
}
