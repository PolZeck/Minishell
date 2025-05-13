/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/13 10:45:24 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	has_too_many_args(char **args)
{
	return (args[1] && args[2]);
}

int	is_exit_argument_valid(char *arg, long long *nb)
{
	if (!str_to_llong(arg, nb))
		return (0);
	if (!is_strict_numeric(arg))
		return (0);
	return (1);
}

void	exit_cleanup(t_cmd *cmd, t_data *data, int exit_code)
{
	free_cmds(cmd);
	free_tokens(data->tokens);
	free_env(data->env);
	rl_clear_history();
	exit(exit_code);
}

int	builtin_exit(t_cmd *cmd, t_data *data)
{
	long long	nb;
	int			code;

	if (!data->in_pipeline)
		ft_putstr_fd("exit\n", 1);
	if (cmd->args[1] && !is_exit_argument_valid(cmd->args[1], &nb))
		handle_invalid_argument(cmd, data);
	if (has_too_many_args(cmd->args))
	{
		handle_too_many_arguments();
		return (1);
	}
	if (cmd->args[1])
		code = (unsigned char)nb;
	else
		code = *get_exit_status();
	*get_exit_status() = code;
	exit_cleanup(cmd, data, code);
	return (0);
}
