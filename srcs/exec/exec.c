/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:29:12 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/19 14:06:39 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	print_err(char *prefix, char *cmd, char *message, int exit_code)
{
	ft_putstr_fd(prefix, STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	*get_exit_status() = exit_code;
}

int	has_valid_command(t_cmd *cmd)
{
	if (cmd->args && cmd->args[0])
		return (1);
	if (cmd->redirs)
		return (1);
	return (0);
}

void	execute_pipeline(t_cmd *cmd_list, t_data *data)
{
	t_cmd	*cmd_tmp;
	int		count;

	count = 0;
	cmd_tmp = cmd_list;
	while (cmd_tmp)
	{
		if (!has_valid_command(cmd_tmp))
		{
			write(2, "minishell: syntax error near unexpected token `newline'\n", 56);
			*get_exit_status() = 2;
			return ;
		}
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
		*get_exit_status() = execute_pipex_direct(cmd_list, data);
	}
}
