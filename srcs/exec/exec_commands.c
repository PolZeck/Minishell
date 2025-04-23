/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:23:02 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/23 12:18:31 by pledieu          ###   ########lyon.fr   */
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
		prefix = ft_strjoin(paths[i], "/");             // chemin/
		full_path = ft_strjoin(prefix, cmd);            // chemin/cmd
		free(prefix);                                   // ✅ libération du intermédiaire
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);                          //  libération du split complet
			return (full_path);                         // on garde le bon path
		}
		free(full_path);                                //  sinon on le libère
		i++;
	}
	free_split(paths); // libère tout
	return (NULL);
}


void print_error(char *prefix, char *cmd, char *message)
{
    ft_putstr_fd(prefix, STDERR_FILENO);
    ft_putstr_fd(cmd, STDERR_FILENO);
    ft_putstr_fd(message, STDERR_FILENO);
}

void	execute_command(t_cmd *cmd, t_data *data)
{
	pid_t	pid;
	char	*cmd_path;
	struct stat	st;

	if (!cmd || !cmd->args || !cmd->args[0])
		return;

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
