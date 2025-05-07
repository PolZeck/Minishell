/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:23:02 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 11:16:58 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command_path(char *cmd, t_data *data)
{
	char	*path;
	char	**paths;
	char	*prefix;
	char	*full_path;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
	path = ft_getenv(data, "PATH");
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

void	print_error(char *prefix, char *cmd, char *message)
{
	ft_putstr_fd(prefix, STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
}

int	validate_redirections(t_cmd *cmd)
{
	t_list	*node;
	t_redir	*redir;
	int		fd;

	node = cmd->redirs;
	while (node)
	{
		redir = node->content;
		if (redir->type == REDIR_IN)
		{
			if (!redir->file && redir->type != HEREDOC)
			{
				fprintf(stderr, "[ERROR] validate_redirections: file is NULL (type = %d)\n", redir->type);
				ft_putstr_fd("minishell: redirection file missing\n", 2);
				return (1);
			}
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				ft_putstr_fd("bash: ", STDERR_FILENO);
				perror(redir->file);
				return (1);
			}
			close(fd);
		}
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			if (!redir->file)
			{
				ft_putstr_fd("minishell: redirection file missing\n", 2);
				return (1);
			}
			fd = open(redir->file, O_WRONLY | O_CREAT
					| (redir->type == APPEND ? O_APPEND : O_TRUNC), 0644);
			if (fd == -1)
			{
				perror(redir->file);
				return (1);
			}
			close(fd);
		}
		node = node->next;
	}
	return (0);
}

void apply_redirections_in_child(t_cmd *cmd)
{
	t_list	*node = cmd->redirs;
	int		input_fd = -1;
	int		output_fd = -1;

	while (node)
	{
		t_redir *redir = node->content;
		int fd = -1;

		if (redir->type == HEREDOC)
		{
			if (redir->fd == -1)
				exit(1);
			if (input_fd != -1)
				close(input_fd);
			input_fd = redir->fd;
		}
		else if (redir->type == REDIR_IN)
		{
			if (!redir->file)
				exit(1);
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				perror(redir->file);
				exit(1);
			}
			if (input_fd != -1)
				close(input_fd);
			input_fd = fd;
		}
		else if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			if (!redir->file)
				exit(1);
			int flags = O_WRONLY | O_CREAT;
			flags |= (redir->type == APPEND) ? O_APPEND : O_TRUNC;
			fd = open(redir->file, flags, 0644);
			if (fd == -1)
			{
				perror(redir->file);
				exit(1);
			}
			if (output_fd != -1)
				close(output_fd);
			output_fd = fd;
		}
		node = node->next;
	}

	if (input_fd != -1)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != -1)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
}

void	execute_command(t_cmd *cmd, t_data *data)
{
	pid_t		pid;
	char		*cmd_path;
	struct stat	st;
	int			saved_stdout;

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
		ft_putstr_fd("bash: : command not found\n", STDERR_FILENO);
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
				ft_putstr_fd("bash: ", STDERR_FILENO);
				ft_putstr_fd(cmd->args[0], STDERR_FILENO);
				ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
				*get_exit_status() = 126;
				return ;
			}
			if (access(cmd->args[0], X_OK) != 0)
			{
				ft_putstr_fd("bash: ", STDERR_FILENO);
				ft_putstr_fd(cmd->args[0], STDERR_FILENO);
				ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
				*get_exit_status() = 126;
				return ;
			}
			cmd_path = ft_strdup(cmd->args[0]);
		}
		else
		{
			ft_putstr_fd("bash: ", STDERR_FILENO);
			ft_putstr_fd(cmd->args[0], STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			*get_exit_status() = 127;
			return ;
		}
	}
	else
	{
		cmd_path = find_command_path(cmd->args[0], data);
		if (!cmd_path)
		{
			ft_putstr_fd("bash: ", STDERR_FILENO);
			ft_putstr_fd(cmd->args[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			*get_exit_status() = 127;
			return ;
		}
	}
	enable_ctrl_backslash();
	saved_stdout = dup(STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		apply_redirections_in_child(cmd);
		if (!cmd->args)
			fprintf(stderr, "[ERROR] cmd->args is NULL\n");
		else
		{
			int i = 0;
			while (cmd->args[i])
				i++;
		}
		execve(cmd_path, cmd->args, data->env);
		perror(cmd_path);
		exit(126);
	}
	else if (pid > 0)
	{
		int status;
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
