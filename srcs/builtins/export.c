/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:32:28 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/29 09:11:36 by pledieu          ###   ########lyon.fr   */
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

static void	print_export(char **env)
{
	int		i;
	char	*eq;
	char	*name;
	char	*value;	

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		eq = ft_strchr(env[i], '=');
		if (eq)
		{
			name = ft_substr(env[i], 0, eq - env[i]);
			value = eq + 1;
			ft_printf("export %s=\"%s\"\n", name, value);
			free(name);
		}
		else
			ft_printf("export %s\n", env[i]);
		i++;
	}
}

int	builtin_export(t_cmd *cmd, t_data *data)
{
	int		i;
	char	**sorted;

	if (!cmd->args[1])
	{
		sorted = dup_env(data->env);
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
