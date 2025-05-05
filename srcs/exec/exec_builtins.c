/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:30:39 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/05 13:28:50 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipex_bonus.h"

void	execute_builtin(t_cmd *cmd, t_data *data)
{
	int		save_stdin;
	int		save_stdout;
	t_list	*node;
	t_redir	*redir;
	int		fd;

	save_stdin = dup(STDIN_FILENO);
	save_stdout = dup(STDOUT_FILENO);

	node = cmd->redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd == -1)
			{
				perror(redir->file);
				*get_exit_status() = 1;
				return ;
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
				perror(redir->file);
				*get_exit_status() = 1;
				return ;
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		node = node->next;
	}
	if (!cmd->args[0])
		return ;
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		*get_exit_status() = builtin_cd(cmd, data);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		*get_exit_status() = builtin_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		*get_exit_status() = builtin_exit(cmd, data);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		*get_exit_status() = builtin_pwd(cmd, data);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		*get_exit_status() = builtin_export(cmd, data);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		*get_exit_status() = builtin_unset(cmd, data);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		*get_exit_status() = builtin_env(cmd, data);
	// ➔ Puis restaurer l'input et l'output d'origine
	dup2(save_stdin, STDIN_FILENO);
	dup2(save_stdout, STDOUT_FILENO);
	close(save_stdin);
	close(save_stdout);
}


void	execute_pipex_builtin(char **args, char **envp, t_pipex *pipex)
{
	t_cmd	cmd;
	t_data	data;

	cmd.args = args;
	cmd.redirs = NULL;
	cmd.invalid = 0;
	cmd.next = NULL;
	data.env = envp;
	execute_builtin(&cmd, &data);
	clean(pipex);
	close_fds(pipex);
	exit(*get_exit_status());
}
