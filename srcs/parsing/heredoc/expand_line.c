/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:17:37 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/15 12:17:51 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	append_char_to_result(char **result, char c)
{
	char	next[2];
	char	*tmp;

	next[0] = c;
	next[1] = '\0';
	tmp = ft_strjoin(*result, next);
	free(*result);
	*result = tmp;
}

static void	expand_variable(const char *line, int *i,
	char **result, t_data *data)
{
	char	*var;
	char	*val;
	char	*tmp;
	char	*to_append;
	int		start;

	start = ++(*i);
	while (ft_isalnum(line[*i]) || line[*i] == '_')
		(*i)++;
	var = ft_substr(line, start, *i - start);
	val = ft_getenv(data, var);
	if (val)
		to_append = ft_strdup(val);
	else
		to_append = ft_strdup("");
	tmp = ft_strjoin(*result, to_append);
	free(*result);
	*result = tmp;
	free(var);
	free(to_append);
}

char	*expand_line(char *line, t_data *data)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1]
			&& (ft_isalnum(line[i + 1]) || line[i + 1] == '_'))
			expand_variable(line, &i, &result, data);
		else
			append_char_to_result(&result, line[i++]);
	}
	return (result);
}
