/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:32:28 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/16 13:36:54 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i = 0;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	// Si y'a un '=', on vérifie qu'il ne précède pas un identifiant invalide
	if (str[i] == '=' && i == 0)
		return (0);
	return (1);
}


static int	var_exists(char **env, char *name)
{
	int		len;
	int		i;

	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static char	**replace_env(char **env, char *new_entry, int idx)
{
	free(env[idx]);
	env[idx] = ft_strdup(new_entry);
	return (env);
}

static char	**append_env(char **env, char *new_entry)
{
	int		i = 0;
	char	**new_env;

	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i++] = ft_strdup(new_entry);
	new_env[i] = NULL;
	free_split(env);
	return (new_env);
}

int	builtin_export(t_cmd *cmd, t_data *data)
{
	int		i;
	char	*eq;
	int		idx;

	if (!cmd->args[1]) // Pas d'argument : rien à faire
		return (*get_exit_status() = 0, 0);

	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			*get_exit_status() = 1;
			i++;
			continue ;
		}
		eq = ft_strchr(cmd->args[i], '=');
		if (eq)
		{
			int len = eq - cmd->args[i];
			char *name = ft_substr(cmd->args[i], 0, len);
			idx = var_exists(data->env, name);
			free(name);
			if (idx >= 0)
				data->env = replace_env(data->env, cmd->args[i], idx);
			else
				data->env = append_env(data->env, cmd->args[i]);
		}
		i++;
	}
	*get_exit_status() = 0;
	return (0);
}
