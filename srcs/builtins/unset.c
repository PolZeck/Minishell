/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:08:44 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/02 12:12:15 by lcosson          ###   ########.fr       */
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
			return (-1); // caractère interdit (hors underscore)
		i++;
	}
	if (has_underscore)
		return (0); // ignorer silencieusement
	return (1); // valide
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
	int	status;

	i = 1;
	status = 0;
	while (cmd->args[i])
	{
		int check = is_valid_identifier_unset(cmd->args[i]);
		if (check == 1)
			data->env = remove_var(data->env, cmd->args[i]);
		else if (check == -1)
		{
			ft_putstr_fd("bash: unset: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		// si check == 0 : on ignore silencieusement
		i++;
	}
	*get_exit_status() = status;
	return (status);
}

