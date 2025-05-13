/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:08:44 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/13 11:54:57 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	is_valid_identifier_unset(char *var)
{
	int	i;

	if (!var || !*var)
		return (0);
	i = 1;
	while (var[i])
	{
		if (!ft_isalnum(var[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	match_var(char *env_var, char *key)
{
	int	i;

	i = 0;
	while (env_var[i] && key[i] && env_var[i] != '=')
	{
		if (env_var[i] != key[i])
			return (0);
		i++;
	}
	return ((env_var[i] == '=' || env_var[i] == '\0') && key[i] == '\0');
}

static int	count_remaining_vars(char **env, char *key)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (env[i])
	{
		if (!match_var(env[i], key))
			count++;
		i++;
	}
	return (count);
}

static char	**copy_env_excluding_key(char **env, char *key, int new_count)
{
	char	**new_env;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_env = malloc(sizeof(char *) * (new_count + 1));
	if (!new_env)
		return (NULL);
	while (env[i])
	{
		if (!match_var(env[i], key))
			new_env[j++] = ft_strdup(env[i]);
		i++;
	}
	new_env[j] = NULL;
	free_split(env);
	return (new_env);
}

int	builtin_unset(t_cmd *cmd, t_data *data)
{
	int	i;
	int	count;

	i = 1;
	while (cmd->args[i])
	{
		if (cmd->args[i][0] == '-')
		{
			ft_putstr_fd("bash: unset: -", 2);
			ft_putchar_fd(cmd->args[i][1], 2);
			ft_putstr_fd(": invalid option\n", 2);
			*get_exit_status() = 2;
			return (2);
		}
		if (is_valid_identifier_unset(cmd->args[i]))
		{
			count = count_remaining_vars(data->env, cmd->args[i]);
			data->env = copy_env_excluding_key(data->env, cmd->args[i], count);
		}
		i++;
	}
	*get_exit_status() = 0;
	return (0);
}
