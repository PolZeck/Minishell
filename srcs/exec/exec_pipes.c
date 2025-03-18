/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:31:04 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/18 12:31:28 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_piped_commands(t_cmd *cmd_list, char **env)
{
	int		pipes[2];
	pid_t	pid;
	t_cmd	*cmd;

	cmd = cmd_list;
	while (cmd)
	{
		pipe(pipes);
		pid = fork();

		if (pid == 0)
		{
			dup2(pipes[1], STDOUT_FILENO);
			close(pipes[0]);
			execute_single_command(cmd, env);
			exit(0);
		}
		else if (pid > 0)
		{
			waitpid(pid, NULL, 0);
			dup2(pipes[0], STDIN_FILENO);
			close(pipes[1]);
		}
		cmd = cmd->next;
	}
}
