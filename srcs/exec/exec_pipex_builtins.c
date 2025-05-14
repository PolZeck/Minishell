/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipex_builtins.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:48:13 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/15 00:43:56 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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
