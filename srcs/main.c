/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:43:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/06 15:15:44 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	debug_print_commands(t_cmd *cmd_list)
{
	int i = 1;
	while (cmd_list)
	{
		printf("=== Command %d ===\n", i);
		for (int j = 0; cmd_list->args && cmd_list->args[j]; j++)
			printf("  Arg[%d]: %s\n", j, cmd_list->args[j]);

		if (cmd_list->redirs)
		{
			t_list *tmp = cmd_list->redirs;
			while (tmp)
			{
				t_redir *r = tmp->content;
				if (r->type == REDIR_IN)
					printf("  <  infile: %s\n", r->file);
				else if (r->type == REDIR_OUT)
					printf("  >  outfile: %s\n", r->file);
				else if (r->type == APPEND)
					printf("  >> append to: %s\n", r->file);
				else if (r->type == HEREDOC)
					printf("  << heredoc delimiter: %s\n", r->file);
				tmp = tmp->next;
			}
		}
		else
			printf("  No redirections.\n");

		if (cmd_list->invalid)
			printf("  ❌ Invalid command!\n");
		cmd_list = cmd_list->next;
		i++;
	}
}

void	debug_tokens(t_token *tokens)
{
	int i = 0;
	while (tokens)
	{
		printf("TOKEN[%d] = [%s], quote_type = %d\n", i, tokens->value, tokens->quote_type);
		tokens = tokens->next;
		i++;
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
	// t_token	*tokens;
	t_cmd	*cmd;
	t_data	data;
	struct termios term;

	(void)argc;
	(void)argv;
	data.env = dup_env(envp, 1);
	if (!data.env)
		return (1);
	setup_signals();
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= ISIG;
	term.c_cc[VQUIT] = 28;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	while (1)
	{
		disable_ctrl_backslash();
		input = readline("minishell> ");
		if (!input)
		{
			write(1, "exit\n", 5);
			free_env(data.env);
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
		data.tokens = tokenize(input, &data);
		// debug_tokens(data.tokens);
		cmd = parse_tokens(data.tokens);
		// debug_print_commands(cmd);
		if (cmd)
			execute_pipeline(cmd, &data);
		free_tokens(data.tokens);
		free_cmds(cmd);
		free(input);
	}
	rl_clear_history();
	return (0);
}
