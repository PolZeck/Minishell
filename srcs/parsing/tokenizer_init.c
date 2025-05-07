/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:00:34 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 11:10:31 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	init_token_structs(t_token_list *tlist)
{
	static t_token	*tokens = NULL;
	static t_token	*last = NULL;

	tokens = NULL;
	last = NULL;
	tlist->tokens = &tokens;
	tlist->last = &last;
}

void	init_parseinfo(t_parseinfo *info, char *input,
	t_data *data, int *i)
{
	*i = 0;
	info->data = data;
	info->input = input;
	info->i = i;
	info->quote = 0;
	info->quote_type = NULL;
	info->next_is_delimiter = 0;
}
