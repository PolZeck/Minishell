/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:20:12 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/10 14:53:54 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

t_cmd *create_cmd()
{
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    cmd->args = malloc(sizeof(char *) * 10); // Limite temporaire à 10 args
    cmd->infile = NULL;
    cmd->outfile = NULL;
    cmd->append = 0;
    cmd->next = NULL;
    return cmd;
}

t_cmd *parse_tokens(t_token *tokens)
{
    t_cmd *cmd = create_cmd();
    t_cmd *head = cmd;
    int arg_count = 0;
    size_t args_size = 2;

    cmd->args = malloc(sizeof(char *) * args_size);
    if (!cmd->args)
        return NULL;

    while (tokens)
    {
        if (tokens->type == WORD || tokens->type == QUOTE) // Commandes et arguments
        {
            if (arg_count + 1 >= (int)args_size) // Vérifier si on doit agrandir
            {
                args_size *= 2;
                cmd->args = ft_realloc(cmd->args, sizeof(char *) * (arg_count + 1), sizeof(char *) * args_size);
                if (!cmd->args)
                    return NULL;
            }
            cmd->args[arg_count++] = ft_strdup(tokens->value);
        }
        else if (tokens->type == PIPE) // Gérer les pipes
        {
            cmd->args[arg_count] = NULL;
            cmd->next = create_cmd();
            cmd = cmd->next;
            arg_count = 0;
            args_size = 2;
            cmd->args = malloc(sizeof(char *) * args_size);
            if (!cmd->args)
                return NULL;
        }
        else if (tokens->type == REDIR_IN) // Gestion de "<"
		{
			tokens = tokens->next;
			if (!tokens)
			{
				ft_printf("Erreur : redirection sans fichier\n");
				return NULL;
			}
			if (tokens->type != WORD)
			{
				ft_printf("Erreur : fichier invalide pour la redirection\n");
				return NULL;
			}
			cmd->infile = ft_strdup(tokens->value);
		}
		else if (tokens->type == REDIR_OUT) // Gestion de ">" et ">>"
		{
			tokens = tokens->next;
			if (!tokens)
			{
				ft_printf("Erreur : redirection sans fichier\n");
				return NULL;
			}
			if (tokens->type != WORD)
			{
				ft_printf("Erreur : fichier invalide pour la redirection\n");
				return NULL;
			}
			cmd->outfile = ft_strdup(tokens->value);
		}
		else if (tokens->type == HEREDOC) // Gestion du "<<"
		{
			tokens = tokens->next;
			if (!tokens)
			{
				ft_printf("Erreur : heredoc sans délimiteur\n");
				return NULL;
			}
			if (tokens->type != WORD)
			{
				ft_printf("Erreur : délimiteur invalide pour le heredoc\n");
				return NULL;
			}
			cmd->infile = ft_strdup(tokens->value);
			ft_printf("Lecture depuis un HEREDOC avec délimiteur : %s\n", cmd->infile);
		}
		else if (tokens->type == APPEND) // Gestion du ">>"
		{
			tokens = tokens->next;
			if (!tokens)
			{
				ft_printf("Erreur : redirection sans fichier\n");
				return NULL;
			}
			if (tokens->type != WORD)
			{
				ft_printf("Erreur : fichier invalide pour la redirection\n");
				return NULL;
			}
			cmd->outfile = ft_strdup(tokens->value);
			cmd->append = 1; // Spécifier que c'est un append
			ft_printf("Écriture en mode APPEND vers : %s\n", cmd->outfile);
		}

        tokens = tokens->next;
    }
    cmd->args[arg_count] = NULL;
    return head;
}

