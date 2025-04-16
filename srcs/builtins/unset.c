/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:08:44 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/16 12:18:34 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	free_split(env); // ou ta fonction perso pour free env
	return (new_env);
}

int	builtin_unset(t_cmd *cmd, t_data *data)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (is_valid_identifier(cmd->args[i]))
			data->env = remove_var(data->env, cmd->args[i]);
		i++;
	}
	*get_exit_status() = 0;
	return (0);
}
