/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:30:39 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/18 15:03:19 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_builtin(t_cmd *cmd)
{
	if (!cmd->args[0])
		return ;
	if (ft_strncmp(cmd->args[0], "cd", 2) == 0)
		builtin_cd(cmd);
	else if (ft_strncmp(cmd->args[0], "echo", 4) == 0)
		builtin_echo(cmd);
	else if (ft_strncmp(cmd->args[0], "exit", 4) == 0)
		builtin_exit(cmd);
}


