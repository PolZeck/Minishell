/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipex_builtins.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:48:13 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/07 10:48:45 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
