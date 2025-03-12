/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:37:53 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/12 08:38:11 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_env_var(char *token, int in_single_quotes)
{
	char	*env_value;

	if (in_single_quotes)
		return (ft_strdup(token));
	if (token[0] != '$')
		return (ft_strdup(token));
	env_value = getenv(token + 1);
	if (env_value)
		return (ft_strdup(env_value));
	return (ft_strdup(""));
}

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = malloc(sizeof(char *) * 10);
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return (cmd);
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
