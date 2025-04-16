/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:29:12 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/16 12:03:38 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	execute_pipeline(t_cmd *cmd_list, t_data *data)
{
	t_cmd	*cmd_tmp;
	int		count = 0;

	cmd_tmp = cmd_list;
	while (cmd_tmp)
	{
		count++;
		cmd_tmp = cmd_tmp->next;
	}
	// ft_printf("DEBUG: count = %d\n", count);

	if (!cmd_list)
	{
		fprintf(stderr, "DEBUG: cmd_list == NULL dans execute_pipeline\n");
		return ;
	}
	// else
	// 	fprintf(stderr, "DEBUG: 1st cmd = %s\n", cmd_list->args ? cmd_list->args[0] : "NULL");

	if (count == 1)
		execute_command(cmd_list, data);
	else
		execute_pipex_adapter(cmd_list, data->env);
	
}

