/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 12:32:28 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/01 14:50:47 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	handle_invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	*get_exit_status() = 1;
	return (0);
}

char	*clean_spaces(const char *str)
{
	char	*res;
	int		i = 0, j = 0;
	int		in_word = 0;

	if (!str)
		return (NULL);
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);

	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
		{
			if (in_word && j > 0 && res[j - 1] == ' ')
				;
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
	return (res);
}

static void	add_or_replace_var(t_data *data, char *arg)
{
	char	*eq;
	char	*name;
	char	*value;
	char	*trimmed;
	char	*final;
	int		idx;
	int		len;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		if (var_exists(data->env, arg) == -1)
			data->env = append_env(data->env, arg);
		return ;
	}
	len = eq - arg;
	name = ft_substr(arg, 0, len);
	value = ft_strdup(eq + 1);
	trimmed = clean_spaces(value);
	free(value);
	final = ft_strjoin(name, "=");
	char *joined = ft_strjoin(final, trimmed);
	free(final);
	free(trimmed);
	idx = var_exists(data->env, name);
	free(name);
	if (idx >= 0)
		data->env = replace_env(data->env, joined, idx);
	else
		data->env = append_env(data->env, joined);
	free(joined);
}

static void	sort_env(char **env)
{
	int		i;
	int		swapped;
	char	*tmp;

	if (!env)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (env[i] && env[i + 1])
		{
			if (ft_strcmp(env[i], env[i + 1]) > 0)
			{
				tmp = env[i];
				env[i] = env[i + 1];
				env[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
	}
}

static void	print_export(char **env)
{
	int		i;
	char	*eq;
	char	*name;
	char	*value;	

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		eq = ft_strchr(env[i], '=');
		if (eq)
		{
			name = ft_substr(env[i], 0, eq - env[i]);
			value = eq + 1;
			printf("export %s=\"%s\"\n", name, value);
			free(name);
		}
		else
			printf("export %s\n", env[i]);
		i++;
	}
}

int	builtin_export(t_cmd *cmd, t_data *data)
{
	int		i;
	char	**sorted;

	if (!cmd->args[1])
	{
		sorted = dup_env(data->env);
		if (!sorted)
			return (1);
		sort_env(sorted);
		print_export(sorted);
		free_split(sorted);
		return (0);
	}
	i = 1;
	while (cmd->args[i])
	{
		if (cmd->args[i][0] == '-')
		{
			ft_putstr_fd("minishell: export: ", 2);
			write(2, cmd->args[i], 2); // écrit juste les 2 premiers caractères : "-X"
			ft_putstr_fd(": invalid option\n", 2);
			*get_exit_status() = 2;
			return (2);
		}
		if (!is_valid_identifier_export(cmd->args[i]))
		{
			handle_invalid_identifier(cmd->args[i]);
			i++;
			continue ;
		}
		add_or_replace_var(data, cmd->args[i]);
		i++;
	}
	return (*get_exit_status());
}
