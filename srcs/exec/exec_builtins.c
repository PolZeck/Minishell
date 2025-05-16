/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:30:39 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/16 15:05:55 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	run_exit_exception(t_cmd *cmd, t_data *data)
{
	if (ft_strcmp(cmd->args[0], "exit") == 0)
	{
		if (!data->in_pipeline)
			run_builtin(cmd, data);
		return (1);
	}
	return (0);
}

void	run_builtin(t_cmd *cmd, t_data *data)
{
	if (!cmd->args[0])
		return ;
	if (ft_strcmp(cmd->args[0], ".") == 0)
	{
		if (!cmd->args[1])
		{
			ft_putstr_fd("bash: .: filename argument required\n", 2);
			*get_exit_status() = 2;
			return ;
		}
	}
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
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
}

void	execute_builtin(t_cmd *cmd, t_data *data)
{
	int	save_stdin;
	int	save_stdout;

	save_stdio(&save_stdin, &save_stdout);
	if (apply_redirs(cmd->redirs))
	{
		restore_stdio(save_stdin, save_stdout);
		return ;
	}
	run_builtin(cmd, data);
	restore_stdio(save_stdin, save_stdout);
}
