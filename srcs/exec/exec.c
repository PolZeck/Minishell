/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:29:12 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/20 09:32:59 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	print_err(char *prefix, char *cmd, char *message)
{
	ft_putstr_fd(prefix, STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
}

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
		return ;
	if (count == 1)
	{
		data->in_pipeline = 0;
		execute_command(cmd_list, data);
	}
	else
	{
		data->in_pipeline = 1;
		data->exit_status = execute_pipex_direct(cmd_list, data);
	}
}
