/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:30:39 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/15 15:51:06 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void	execute_builtin(t_cmd *cmd)
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
	{
		ft_printf("bientôt : export\n");
		*get_exit_status() = 0;
	}
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
	{
		ft_printf("bientôt : unset\n");
		*get_exit_status() = 0;
	}
	else if (ft_strcmp(cmd->args[0], "env") == 0)
	{
		ft_printf("bientôt : env\n");
		*get_exit_status() = 0;
	}
}
