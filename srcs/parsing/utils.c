/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:37:53 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/10 11:02:32 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void free_tokens(t_token *tokens)
{
    t_token *tmp;

    while (tokens)
    {
        tmp = tokens->next;
        if (tokens->value)
            free(tokens->value);
        free(tokens);
        tokens = tmp;
    }
}

void free_cmds(t_cmd *cmds)
{
    int i = 0;

    if (!cmds)
        return;
    if (cmds->args)
    {
        while (cmds->args[i])
        {
            free(cmds->args[i]);
            i++;
        }
        free(cmds->args);
    }
    free(cmds);
}

void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
    void *new_ptr;
    size_t i;

    if (new_size == 0)
    {
        free(ptr);
        return NULL;
    }
    new_ptr = malloc(new_size);
    if (!new_ptr)
        return NULL;

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
    return new_ptr;
}

char *expand_env_var(char *token)
{
    if (token[0] != '$')
        return ft_strdup(token);
    
    char *env_value = getenv(token + 1);
    return env_value ? ft_strdup(env_value) : ft_strdup("");
}
