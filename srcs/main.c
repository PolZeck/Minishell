/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:43:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/17 16:22:47 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	display_parsed_commands(t_cmd *cmd)
{
	t_cmd	*current;
	int		i;

	current = cmd;

	while (current)
	{
		if (!current->args || !current->args[0])
		{
			current = current->next;
			continue;
		}
		ft_printf("Commande : %s\n", current->args[0]);
		i = 1;
		while (current->args[i])
		{
			ft_printf("Arg[%d]: %s\n", i, current->args[i]);
			i++;
		}
		if (current->infile)
		{
			if (current->append == -1)
				ft_printf("Lecture depuis un HEREDOC avec délimiteur : %s\n",
					current->infile);
			else
				ft_printf("Lecture depuis : %s\n", current->infile);
		}
		if (current->outfile)
			ft_printf("Écriture vers : %s (Append : %d)\n",
				current->outfile, current->append);
		if (current->next)
			ft_printf("⬇ Pipe vers la prochaine commande ⬇\n");
		current = current->next;
	}
}

int main(void)
{
    char *input;
    t_token *tokens;
    t_cmd *cmd;

    setup_signals(); // Active la gestion des signaux

    while (1)
    {
        input = readline("minishell> ");
        if (!input) // Gestion de Ctrl-D
        {
            write(1, "exit\n", 5);
			rl_clear_history();
            break;
        }
        add_history(input);
        if (check_unclosed_quotes(input))
        {
            perror("Erreur : guillemets non fermés");
            free(input);
            continue;
        }
        tokens = tokenize(input);
        cmd = parse_tokens(tokens);
		if (!cmd)
		{
			free_tokens(tokens);
			free(input);
			continue;
		}
		t_cmd *tmp = cmd;
		while (tmp)
		{
			ft_printf("Commande : %s | Invalid : %d\n", tmp->args[0], tmp->invalid);
			tmp = tmp->next;
		}

        display_parsed_commands(cmd);
        free_tokens(tokens);
        free_cmds(cmd);
        free(input);
    }
	rl_clear_history();
    return (0);
}