/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:32:28 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/23 13:26:19 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	handle_invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	*get_exit_status() = 1;
	return (0);
}

static void	add_or_replace_var(t_data *data, char *arg)
{
	char	*eq;
	char	*name;
	int		idx;
	int		len;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return ;
	len = eq - arg;
	name = ft_substr(arg, 0, len);
	idx = var_exists(data->env, name);
	free(name);
	if (idx >= 0)
		data->env = replace_env(data->env, arg, idx);
	else
		data->env = append_env(data->env, arg);
}

int	builtin_export(t_cmd *cmd, t_data *data)
{
	int	i;

	if (!cmd->args[1])
	{
		*get_exit_status() = 0;
		return (0);
	}
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_identifier_export(cmd->args[i]))
		{
			handle_invalid_identifier(cmd->args[i]);
			i++;
			continue ;
		}
		add_or_replace_var(data, cmd->args[i]);
		i++;
	}
	return (*get_exit_status());
}
