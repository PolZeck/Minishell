/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:08:44 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/17 15:06:21 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	is_valid_identifier(char *var)
{
	int	i = 0;

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

static char	**remove_var(char **env, char *key)
{
	int		i = 0;
	int		j = 0;
	int		count = 0;
	char	**new_env;

	while (env[count])
		count++;
	new_env = malloc(sizeof(char *) * count);
	if (!new_env)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0 && env[i][ft_strlen(key)] == '=')
			i++;
		else
			new_env[j++] = ft_strdup(env[i++]);
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
		if (!is_valid_identifier(cmd->args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			*get_exit_status() = 1;
		}
		else
			data->env = remove_var(data->env, cmd->args[i]);
		i++;
	}
	// ✅ code retour = 0 seulement si aucune erreur
	if (*get_exit_status() != 1)
		*get_exit_status() = 0;
	return (*get_exit_status());
}
