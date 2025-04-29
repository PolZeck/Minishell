/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:08:44 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/29 16:27:30 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	is_valid_identifier_unset(char *var)
{
	int	i;

	i = 0;
	if (!var || !ft_isalpha(var[0]))
		return (0);
	while (var[i])
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
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
	return (env_var[i] == '=' && key[i] == '\0');
}

static char	**remove_var(char **env, char *key)
{
	int		i;
	int		j;
	int		new_count;
	char	**new_env;

	new_count = 0;
	i = 0;
	while (env[i])
	{
		if (!match_var(env[i], key))
			new_count++;
		i++;
	}
	new_env = malloc(sizeof(char *) * (new_count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
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

	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_identifier_unset(cmd->args[i]))
		{
			ft_putstr_fd("bash: unset: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			*get_exit_status() = 1;
		}
		else
			data->env = remove_var(data->env, cmd->args[i]);
		i++;
	}
	if (*get_exit_status() != 1)
		*get_exit_status() = 0;
	return (*get_exit_status());
}
