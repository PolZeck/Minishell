/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:20 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/27 16:07:47 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void builtin_cd(t_cmd *cmd)
{
	if (cmd->args[2])
	{
		ft_printf("bash: cd: too many arguments\n");
		return ;
	}
	else if (cmd->args[1] == NULL)
	{
		ft_printf("minishell: cd: missing argument\n");
		return ;
	}
	else if (chdir(cmd->args[1]) != 0)
		perror("minishell: cd");
	return ;
}
