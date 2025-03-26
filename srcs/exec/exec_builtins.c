/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:30:39 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/26 11:06:28 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_builtin(t_cmd *cmd)
{
	if (!cmd->args[0])
		return ;
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		builtin_cd(cmd);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		builtin_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		ft_printf("bientôt : pwd\n");
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		ft_printf("bientôt : export\n");
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		ft_printf("bientôt : unset\n");
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		ft_printf("bientôt : env\n");
}
