/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:04:19 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/06 15:24:26 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

/* void	sort_env(char **env)
{
	int		i;
	int		swapped;
	char	*tmp;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (env[i] && env[i + 1])
		{
			if (ft_strcmp(env[i], env[i + 1]) > 0)
			{
				tmp = env[i];
				env[i] = env[i + 1];
				env[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
	}
}

void	print_export(char **env)
{
	int		i;
	char	*eq;
	char	*name;
	char	*value;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "_=", 2) != 0)
		{
			eq = ft_strchr(env[i], '=');
			if (eq)
			{
				name = ft_substr(env[i], 0, eq - env[i]);
				value = eq + 1;
				printf("export %s=\"%s\"\n", name, value);
				free(name);
			}
			else
				printf("export %s\n", env[i]);
		}
		i++;
	}
}

static int	handle_export_option(char *arg)
{
	ft_putstr_fd("minishell: export: ", 2);
	write(2, arg, 2);
	ft_putstr_fd(": invalid option\n", 2);
	*get_exit_status() = 2;
	return (2);
}

static int	handle_export_args(t_cmd *cmd, t_data *data)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (cmd->args[i][0] == '-')
			return (handle_export_option(cmd->args[i]));
		if (!is_valid_identifier_export(cmd->args[i]))
			handle_invalid_identifier(cmd->args[i]);
		else
			add_or_replace_var(data, cmd->args[i]);
		i++;
	}
	return (*get_exit_status());
}

int	builtin_export(t_cmd *cmd, t_data *data)
{
	char	**sorted;

	if (!cmd->args[1])
	{
		sorted = dup_env(data->env, 0);
		if (!sorted)
			return (1);
		sort_env(sorted);
		print_export(sorted);
		free_split(sorted);
		return (0);
	}
	return (handle_export_args(cmd, data));
} */
