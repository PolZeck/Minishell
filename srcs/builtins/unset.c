/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:08:44 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/06 14:55:40 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	is_valid_identifier_unset(char *var)
{
	int	i;
	int	has_underscore;

	if (!var || !*var)
		return (0);
	has_underscore = 0;
	i = 0;
	while (var[i])
	{
		if (var[i] == '_')
			has_underscore = 1;
		else if (!ft_isalnum(var[i]))
			return (-1);
		i++;
	}
	if (has_underscore)
		return (0);
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
	if (env_var[i] == '=' && key[i] == '\0')
		return (1);
	return (0);
}

static int	count_remaining_vars(char **env, char *key)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
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

	new_env = malloc(sizeof(char *) * (new_count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (!match_var(env[i], key))
		{
			new_env[j] = ft_strdup(env[i]);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	free_split(env);
	return (new_env);
}

int	builtin_unset(t_cmd *cmd, t_data *data)
{
	int	i;
	int	valid;
	int	count;

	i = 1;
	while (cmd->args[i])
	{
		valid = is_valid_identifier_unset(cmd->args[i]);
		if (!valid)
		{
			ft_putstr_fd("bash: unset: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			*get_exit_status() = 1;
		}
		else
		{
			count = count_remaining_vars(data->env, cmd->args[i]);
			data->env = copy_env_excluding_key(data->env, cmd->args[i], count);
		}
		i++;
	}
	if (*get_exit_status() != 1)
		*get_exit_status() = 0;
	return (*get_exit_status());
}
