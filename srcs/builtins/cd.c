/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:20 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/18 16:22:17 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void builtin_cd(t_cmd *cmd)
{
	if (cmd->args[1] == NULL)
	{
		ft_printf("minishell: cd: missing argument\n");
		return;
	}
	if (chdir(cmd->args[1]) != 0)
		perror("minishell: cd");
}
