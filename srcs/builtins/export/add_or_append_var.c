/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_or_append_var.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:00:41 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/15 12:01:09 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	replace_existing_var(t_data *data, int idx,
	char *name, char *value)
{
	char	*old;
	char	*temp;
	char	*joined;

	old = ft_strdup(ft_strchr(data->env[idx], '=') + 1);
	temp = ft_strjoin(old, value);
	free(old);
	joined = ft_strjoin(name, "=");
	free(data->env[idx]);
	data->env[idx] = ft_strjoin(joined, temp);
	free(joined);
	free(temp);
}

static void	add_new_var(t_data *data, char *name, char *value)
{
	char	*joined;
	char	*entry;

	joined = ft_strjoin(name, "=");
	entry = ft_strjoin(joined, value);
	data->env = append_env(data->env, entry);
	free(joined);
	free(entry);
}

void	add_or_append_var(t_data *data, char *arg)
{
	char	*name;
	char	*value;
	int		idx;

	name = ft_substr(arg, 0, ft_strchr(arg, '+') - arg);
	value = ft_strdup(ft_strchr(arg, '=') + 1);
	idx = var_exists(data->env, name);
	if (idx >= 0)
		replace_existing_var(data, idx, name, value);
	else
		add_new_var(data, name, value);
	free(name);
	free(value);
}
