/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:43:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/19 11:19:26 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
	fonction main, entrée du programme,
	gere qu'il n'ya bien pas d'input pour le ctrl + D
	puis tokenize et parse l'input,
	en enfin envoie execute_pipeline avec notre commande et l'envp
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
		if (!cmd)
		{
			free_tokens(tokens);
			free(input);
			continue ;
		}
		free_tokens(tokens);
		free_cmds(cmd);
		free(input);
	}
	rl_clear_history();
	return (0);
}
