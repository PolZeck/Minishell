/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:29:12 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/06 07:49:05 by lcosson          ###   ########.fr       */
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
	{
		data->in_pipeline = 0;
		execute_command(cmd_list, data);
	}
	else
	{
		data->in_pipeline = 1;
		*get_exit_status() = execute_pipex_direct(cmd_list, data);
	}
}
