/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:32:28 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 10:21:56 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	sort_env(char **env)
{
	int		i;
	int		swapped;
	char	*tmp;

	if (!env)
		return ;
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

static void	print_one_export(char *entry)
{
	char	*eq;
	char	*name;
	char	*value;

	eq = ft_strchr(entry, '=');
	if (eq)
	{
		name = ft_substr(entry, 0, eq - entry);
		value = eq + 1;
		ft_printf("export %s=\"%s\"\n", name, value);
		free(name);
	}
	else
		ft_printf("export %s\n", entry);
}

static void	print_export(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "_=", 2) != 0)
			print_one_export(env[i]);
		i++;
	}
}

static int	handle_export_arg(char *arg, t_data *data)
{
	if (arg[0] == '-')
	{
		ft_putstr_fd("minishell: export: ", 2);
		write(2, arg, 2);
		ft_putstr_fd(": invalid option\n", 2);
		*get_exit_status() = 2;
		return (2);
	}
	if (!is_valid_identifier_export(arg))
	{
		handle_invalid_identifier(arg);
		return (1);
	}
	add_or_replace_var(data, arg);
	return (0);
}

int	builtin_export(t_cmd *cmd, t_data *data)
{
	int		i;
	char	**sorted;
	int		ret;

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
	i = 1;
	while (cmd->args[i])
	{
		ret = handle_export_arg(cmd->args[i], data);
		if (ret == 2)
			return (2);
		i++;
	}
	return (*get_exit_status());
}
