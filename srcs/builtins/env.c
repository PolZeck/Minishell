/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:20:10 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/19 13:34:33 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_env(t_cmd *cmd, t_data *data)
{
	int	i;

	if (cmd->args[1])
	{
		ft_putstr_fd("env: No arguments allowed\n", 2);
		data->exit_status = 1;
		return (1);
	}
	i = 0;
	while (data->env[i])
	{
		if (ft_strchr(data->env[i], '='))
		{
			ft_putstr_fd(data->env[i], 1);
			ft_putstr_fd("\n", 1);
		}
		i++;
	}
	data->exit_status = 0;
	return (0);
}
