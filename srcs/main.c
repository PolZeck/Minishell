/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:43:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/26 12:31:35 by pledieu          ###   ########lyon.fr   */
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

/*
	fonction main, entrée du programme,
	gere qu'il n'ya bien pas d'input pour le ctrl + D
	puis tokenize et parse l'input,
	et enfin envoie execute_pipeline avec notre commande et l'envp
*/
int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmd;

	(void)argc;
	(void)argv;
	setup_signals();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			write(1, "exit\n", 5);
			rl_clear_history();
			break ;
		}
		add_history(input);
		if (check_unclosed_quotes(input))
		{
			perror("Erreur : guillemets non fermés");
			free(input);
			continue ;
		}
		tokens = tokenize(input);
		cmd = parse_tokens(tokens);
		if (cmd)
			execute_pipeline(cmd, envp);
		// cmd = parse_tokens(tokens);

		// 🔥 DEBUG ONLY : Affichage des commandes parsées (en attendant exec)
		// if (cmd)
		// {
		// 	t_cmd *tmp = cmd;
		// 	int c = 1;
		// 	while (tmp)
		// 	{
		// 		if (tmp->args && tmp->args[0])
		// 		{
		// 			ft_printf("🔹 Commande %d : %s\n", c, tmp->args[0]);
		// 			for (int i = 1; tmp->args[i]; i++)
		// 				ft_printf("   Arg[%d]: %s\n", i, tmp->args[i]);
		// 		}
		// 		else
		// 			ft_printf("🔹 Commande %d vide\n", c);
		// 		tmp = tmp->next;
		// 		c++;
		// 	}
		// }

		if (!cmd)
		{
			free_tokens(tokens);
			free(input);
			continue ;
		}
		// display_parsed_commands(cmd);
		free_tokens(tokens);
		free_cmds(cmd);
		free(input);
	}
	rl_clear_history();
	return (0);
}
