/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:19:52 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 11:04:01 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*tokenize_loop(char *buffer,
	t_parseinfo *info, t_token_list *tlist)
{
	t_token	*tokens;

	*tlist->tokens = NULL;
	*tlist->last = NULL;
	while (info->input[*(info->i)])
		handle_input_token(tlist, &buffer, info);
	if (*buffer)
		flush_buffer_to_token(*tlist, &buffer, NO_QUOTE, info);
	tokens = *tlist->tokens;
	free(buffer);
	return (tokens);
}

t_token	*tokenize(char *input, t_data *data)
{
	t_parseinfo		info;
	t_token_list	tlist;
	char			*buffer;
	int				i;

	buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	i = 0;
	init_parseinfo(&info, input, data, &i);
	init_token_structs(&tlist);
	return (tokenize_loop(buffer, &info, &tlist));
}
