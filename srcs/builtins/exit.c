/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/18 15:04:26 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void builtin_exit(t_cmd *cmd)
{
	int status = 0;

    if (cmd->args[1])
        status = ft_atoi(cmd->args[1]);
    ft_printf("exit\n");
        free_cmds(cmd);
    exit(status);
}
