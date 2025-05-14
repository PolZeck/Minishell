/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variable_expansion_tokenizer.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 00:22:47 by pol               #+#    #+#             */
/*   Updated: 2025/05/15 00:24:18 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	expand_as_delimiter(char **buffer, t_parseinfo *info)
{
	int		i;
	int		len;
	char	*raw;
	char	*tmp;

	len = 1;
	i = *(info->i);
	while (info->input[i + len]
		&& (ft_isalnum(info->input[i + len]) || info->input[i + len] == '_'))
		len++;
	raw = ft_substr(info->input, i, len);
	tmp = ft_strjoin(*buffer, raw);
	free(*buffer);
	*buffer = tmp;
	free(raw);
	*(info->i) += len;
	info->next_is_delimiter = 0;
}

static int	append_dollar_if_alone(char **buffer, t_parseinfo *info)
{
	char	*tmp;

	if (!info->input[*(info->i) + 1]
		|| (!ft_isalnum(info->input[*(info->i) + 1])
			&& info->input[*(info->i) + 1] != '_'
			&& info->input[*(info->i) + 1] != '?'))
	{
		tmp = ft_strjoin(*buffer, "$");
		free(*buffer);
		*buffer = tmp;
		(*info->i)++;
		return (1);
	}
	return (0);
}

static char	*get_expanded_value(t_parseinfo *info)
{
	char	*var;
	char	*value;
	int		start;

	if (info->input[*(info->i)] == '?')
	{
		(*info->i)++;
		return (ft_itoa(*get_exit_status()));
	}
	start = *(info->i);
	while (ft_isalnum(info->input[*(info->i)])
		|| info->input[*(info->i)] == '_')
		(*info->i)++;
	var = ft_substr(info->input, start, *(info->i) - start);
	value = ft_getenv(info->data, var);
	free(var);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static void	append_expanded_value(char **buffer, char *value)
{
	char	*tmp;

	tmp = ft_strjoin(*buffer, value);
	free(*buffer);
	*buffer = tmp;
	free(value);
}

void	handle_variable_expansion_tokenizer(char **buffer, t_parseinfo *info)
{
	char	*value;

	if (info->next_is_delimiter)
		return (expand_as_delimiter(buffer, info));
	if (append_dollar_if_alone(buffer, info))
		return ;
	(*info->i)++;
	value = get_expanded_value(info);
	if (info->quote_type && *(info->quote_type) != NO_QUOTE)
		return (append_expanded_value(buffer, value));
	append_expanded_value(buffer, value);
}
