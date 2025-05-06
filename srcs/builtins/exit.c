/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/06 07:49:23 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <limits.h>

static int	is_strict_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	str_to_llong(const char *str, long long *out)
{
	int			sign;
	long long	result;
	int			i;
	int			digit;

	sign = 1;
	result = 0;
	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		digit = str[i++] - '0';
		if (sign == 1 && (result > (LLONG_MAX - digit) / 10))
			return (0);
		if (sign == -1 && (-result < (LLONG_MIN + digit) / 10))
			return (0);
		result = result * 10 + digit;
	}
	*out = result * sign;
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
	long long	exit_ll;
	int			exit_code;

	if (!data->in_pipeline)
		ft_putstr_fd("exit\n", 1);
	if (cmd->args[1] && (!is_strict_numeric(cmd->args[1])
			|| !str_to_llong(cmd->args[1], &exit_ll)))
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
		exit_code = (unsigned char)exit_ll;
	else
		exit_code = 0;
	*get_exit_status() = exit_code;
	exit_cleanup(cmd, data, exit_code);
	return (0);
}
