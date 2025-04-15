/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/15 12:49:32 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

int	is_numeric_arg(char *str)
{
	int	i = 0;

	if (!str)
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	print_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

int	builtin_exit(t_cmd *cmd)
{
	int	exit_code;

	ft_printf("exit\n");

	if (cmd->args[1] && !is_numeric_arg(cmd->args[1]))
	{
		print_numeric_error(cmd->args[1]);
		*get_exit_status() = 2;
		free_cmds(cmd);
		exit(2);
	}

	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		*get_exit_status() = 1;
		return (1); // ne quitte pas
	}

	exit_code = 0;
	if (cmd->args[1])
		exit_code = ft_atoi(cmd->args[1]);
	*get_exit_status() = exit_code;
	free_cmds(cmd);
	exit(exit_code);
}
