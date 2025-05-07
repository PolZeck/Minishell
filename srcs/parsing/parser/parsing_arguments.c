/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_arguments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:08:19 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 09:08:39 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	handle_argument(t_cmd *cmd, int *arg_count, char *arg)
{
	int		new_size;
	char	**new_args;
	int		i;

	new_size = (*arg_count) + 2;
	new_args = malloc(sizeof(char *) * new_size);
	if (!new_args)
		return ;
	i = 0;
	while (i < *arg_count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	(*arg_count)++;
}
