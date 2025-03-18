/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:34 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/18 15:03:13 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void builtin_echo(t_cmd *cmd)
{
    int i = 1;

    if (cmd->args[1] && ft_strncmp(cmd->args[1], "-n", 2) == 0)
        i = 2;

    while (cmd->args[i])
    {
        ft_printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
            ft_printf(" ");
        i++;
    }
    if (cmd->args[1] && ft_strncmp(cmd->args[1], "-n", 2) != 0)
        ft_printf("\n");
}
