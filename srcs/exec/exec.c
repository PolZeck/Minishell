/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:29:12 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/28 13:51:11 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pipeline(t_cmd *cmd_list, t_data *data)
{
	t_cmd	*cmd_tmp;
	int		count;

	count = 0;
	cmd_tmp = cmd_list;
	while (cmd_tmp)
	{
		count++;
		cmd_tmp = cmd_tmp->next;
	}
	if (!cmd_list)
	{
		fprintf(stderr, "DEBUG: cmd_list == NULL dans execute_pipeline\n");
		return ;
	}
	if (count == 1)
		execute_command(cmd_list, data);
	else
		*get_exit_status() = execute_pipex_direct(cmd_list, data);
}
