/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:30:39 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/16 12:33:31 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void	execute_builtin(t_cmd *cmd,  t_data *data)
{
	if (!cmd->args[0])
		return ;
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		*get_exit_status() = builtin_cd(cmd);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		*get_exit_status() = builtin_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		*get_exit_status() = builtin_exit(cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		*get_exit_status() = builtin_pwd(cmd);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		*get_exit_status() = builtin_export(cmd, data);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		*get_exit_status() = builtin_unset(cmd, data);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		*get_exit_status() = builtin_env(cmd, data);
}
