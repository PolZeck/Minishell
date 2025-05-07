/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variable_expansion.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 08:32:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 08:32:43 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	append_raw_variable(char **buffer, char *input, int *i)
{
	int		start;
	char	*raw;
	char	*tmp;

	start = *i;
	(*i)++;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	raw = ft_substr(input, start, *i - start);
	tmp = ft_strjoin(*buffer, raw);
	free(*buffer);
	free(raw);
	*buffer = tmp;
}

static int	handle_digit_expansion(char **buffer, char *input, int *i)
{
	char	tmp_str[2];
	char	*joined;

	if (!ft_isdigit(input[*i]))
		return (0);
	(*i)++;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
	{
		tmp_str[0] = input[*i];
		tmp_str[1] = '\0';
		joined = ft_strjoin(*buffer, tmp_str);
		free(*buffer);
		*buffer = joined;
		(*i)++;
	}
	return (1);
}

static char	*extract_variable(char *input, int *i)
{
	int		start;
	char	*var_name;
	char	*var;

	if (input[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("$?"));
	}
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var_name = ft_substr(input, start, *i - start);
	var = ft_strjoin("$", var_name);
	free(var_name);
	return (var);
}

void	handle_variable_expansion(char **buffer,
	char *input, int *i, t_parseinfo *info)
{
	char	*var;
	char	*value;
	char	*to_append;
	char	*tmp;

	if (info && info->next_is_delimiter)
		return (append_raw_variable(buffer, input, i));
	if (input[*i] == '"' || input[*i] == '\'')
		return ;
	(*i)++;
	if (handle_digit_expansion(buffer, input, i))
		return ;
	var = extract_variable(input, i);
	value = expand_env_var(var, 0, info->data);
	to_append = ft_strdup(value);
	tmp = ft_strjoin(*buffer, to_append);
	free(*buffer);
	free(var);
	free(to_append);
	free(value);
	*buffer = tmp;
}
