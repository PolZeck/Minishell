/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 09:08:52 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 09:11:11 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	handle_pipe(t_cmd **cmd, int *arg_count,
	size_t *args_size, t_token **tokens)
{
	(*cmd)->args[*arg_count] = NULL;
	if (!(*tokens)->next || (*tokens)->next->type == PIPE)
	{
		ft_printf("bash: syntax error near unexpected token '|'");
		*tokens = NULL;
		return ;
	}
	*tokens = (*tokens)->next;
	(*cmd)->next = create_cmd(*tokens);
	if (!(*cmd)->next)
		return ;
	(*cmd) = (*cmd)->next;
	*arg_count = 0;
	*args_size = 2;
	(*cmd)->args = malloc(sizeof(char *) * (*args_size));
	if (!(*cmd)->args)
		return ;
}
