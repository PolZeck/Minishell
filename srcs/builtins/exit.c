/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/25 15:40:20 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	is_numeric_arg(char *str)
{
	int	i;

	i = 0;
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
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

static void	exit_cleanup(t_cmd *cmd, t_data *data, int exit_code)
{
	free_cmds(cmd);
	free_tokens(data->tokens);
	free_env(data->env);
	rl_clear_history();
	exit(exit_code);
}

int	builtin_exit(t_cmd *cmd, t_data *data)
{
	int	exit_code;

	ft_putstr_fd("exit\n", 1);
	if (cmd->args[1] && !is_numeric_arg(cmd->args[1]))
	{
		print_numeric_error(cmd->args[1]);
		*get_exit_status() = 2;
		exit_cleanup(cmd, data, 2);
	}
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		*get_exit_status() = 1;
		return (1);
	}
	if (cmd->args[1])
		exit_code = ft_atoi(cmd->args[1]);
	else
		exit_code = 0;
	*get_exit_status() = exit_code;
	exit_cleanup(cmd, data, exit_code);
	return (0);
}
