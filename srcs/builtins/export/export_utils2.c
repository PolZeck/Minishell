/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:04:19 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/15 12:01:01 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	handle_invalid_identifier(char *arg)
{
	ft_putstr_fd("bash: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	*get_exit_status() = 1;
	return (0);
}

static char	*get_cleaned_assignment(char *arg, char **name)
{
	char	*eq;
	char	*value;
	char	*trimmed;
	char	*prefix;
	char	*result;

	eq = ft_strchr(arg, '=');
	*name = ft_substr(arg, 0, eq - arg);
	value = ft_strdup(eq + 1);
	trimmed = ft_strdup(value);
	free(value);
	prefix = ft_strjoin(*name, "=");
	result = ft_strjoin(prefix, trimmed);
	free(prefix);
	free(trimmed);
	return (result);
}

void	add_or_replace_var(t_data *data, char *arg)
{
	char	*name;
	char	*cleaned;
	int		idx;

	if (!ft_strchr(arg, '='))
	{
		if (var_exists(data->env, arg) == -1)
			data->env = append_env(data->env, arg);
		return ;
	}
	cleaned = get_cleaned_assignment(arg, &name);
	idx = var_exists(data->env, name);
	if (idx >= 0)
		data->env = replace_env(data->env, cleaned, idx);
	else
		data->env = append_env(data->env, cleaned);
	free(name);
	free(cleaned);
}
