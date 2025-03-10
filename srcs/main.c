/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:43:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/10 14:30:22 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(void)
{
    char *input;

    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break;

        add_history(input);
        if (check_unclosed_quotes(input))
        {
            ft_printf("Erreur : guillemets non fermés\n");
            free(input);
            continue;
        }

        t_token *tokens = tokenize(input);
        t_cmd *cmd = parse_tokens(tokens);

        if (cmd)
        {
            ft_printf("Commande : %s\n", cmd->args[0]);
            for (int i = 1; cmd->args[i]; i++)
                ft_printf("Arg[%d]: %s\n", i, cmd->args[i]);

            if (cmd->infile)
            {
                if (cmd->append == -1) // Si append == -1, c'est un HEREDOC
                    ft_printf("Lecture depuis un HEREDOC avec délimiteur : %s\n", cmd->infile);
                else
                    ft_printf("Lecture depuis : %s\n", cmd->infile);
            }

            if (cmd->outfile)
                ft_printf("Écriture vers : %s (Append : %d)\n", cmd->outfile, cmd->append);
        }

        free_tokens(tokens);
        free_cmds(cmd);
        free(input);
    }
    return (0);
}
