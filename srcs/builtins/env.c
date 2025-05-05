/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:20:10 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/05 11:46:08 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_env(t_cmd *cmd, t_data *data)
{
	int	i;

	if (cmd->args[1])
	{
		ft_putstr_fd("env: No arguments allowed\n", 2);
		*get_exit_status() = 1;
		return (1);
	}
	i = 0;
	while (data->env[i])
	{
		if (ft_strchr(data->env[i], '=')) // ✅ Affiche uniquement si '=' présent
		{
			ft_putstr_fd(data->env[i], 1);
			ft_putstr_fd("\n", 1);
		}
		i++;
	}
	*get_exit_status() = 0;
	return (0);
}

