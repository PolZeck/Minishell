/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipex_adapter.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:50:45 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/09 10:19:26 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_cmds(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

static int	init_pipex_direct(t_pipex *pipex, t_cmd *cmds)
{
	ft_memset(pipex, 0, sizeof(t_pipex));
	pipex->in_fd = STDIN_FILENO;
	pipex->out_fd = STDOUT_FILENO;
	pipex->num_cmds = count_cmds(cmds);
	pipex->pid = malloc(sizeof(pid_t) * pipex->num_cmds);
	if (!pipex->pid)
		return (0);
	return (1);
}

static void	launch_pipex_commands(t_pipex *pipex, t_cmd *cmds, t_data *data)
{
	t_cmd	*current;
	int		i;

	current = cmds;
	i = 0;
	while (current)
	{
		pipex->current_cmd = current;
		if (i == 0)
			pipex->pid[i] = first_execution_direct(pipex, data);
		else if (i == pipex->num_cmds - 1)
			pipex->pid[i] = last_execution_direct(pipex, data);
		else
			pipex->pid[i] = middle_execution_direct(pipex, data);
		current = current->next;
		i++;
	}
}

int	execute_pipex_direct(t_cmd *cmds, t_data *data)
{
	t_pipex	pipex;
	int		exit_code;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (!init_pipex_direct(&pipex, cmds))
		return (1);
	launch_pipex_commands(&pipex, cmds, data);
	close_fds(&pipex);
	exit_code = wait_for_processes(&pipex);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	return (exit_code);
}
