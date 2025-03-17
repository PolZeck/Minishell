/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 07:45:17 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/17 13:46:15 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	i;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		i = 0;
		while (i < old_size && i < new_size)
		{
			((char *)new_ptr)[i] = ((char *)ptr)[i];
			i++;
		}
		free(ptr);
	}
	return (new_ptr);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

void free_cmds(t_cmd *cmd)
{
    t_cmd *tmp;
    int i;

    while (cmd)
    {
        tmp = cmd->next;
        i = 0;
        if (cmd->args)
        {
            while (cmd->args[i])
                free(cmd->args[i++]);
            free(cmd->args);
        }
        if (cmd->infile)
            free(cmd->infile);
        if (cmd->outfile)
            free(cmd->outfile);
        if(cmd)
        	free(cmd);  // ✅ Vérifie que `cmd` existe avant de le libérer
        cmd = tmp;
    }
}
