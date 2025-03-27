/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:34 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/27 16:07:44 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void builtin_echo(t_cmd *cmd)
{
    int i = 1;

    if (cmd->args[1] && ft_strcmp(cmd->args[1], "-n") == 0)
        i = 2;

    while (cmd->args[i])
    {
        ft_printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
            ft_printf(" ");
        i++;
    }
    if (cmd->args[1] && ft_strcmp(cmd->args[1], "-n") != 0)
        ft_printf("\n");
	
}
