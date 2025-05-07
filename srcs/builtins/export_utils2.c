/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:04:19 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/07 10:15:39 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	copy_cleaned(const char *str, char *res)
{
	int	i;
	int	j;
	int	in_word;

	i = 0;
	j = 0;
	in_word = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
		{
			res[j++] = str[i];
			in_word = 1;
		}
		else if (in_word)
		{
			res[j++] = ' ';
			in_word = 0;
		}
		i++;
	}
	if (j > 0 && res[j - 1] == ' ')
		j--;
	res[j] = '\0';
}

char	*clean_spaces(const char *str)
{
	char	*res;

	if (!str)
		return (NULL);
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	copy_cleaned(str, res);
	return (res);
}

int	handle_invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
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
	trimmed = clean_spaces(value);
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
