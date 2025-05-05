/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:43:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/05 17:04:50 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	display_parsed_commands(t_cmd *cmd)
// {
// 	t_cmd	*current;
// 	int		i;

// 	current = cmd;

// 	while (current)
// 	{
// 		if (!current->args || !current->args[0])
// 		{
// 			current = current->next;
// 			continue;
// 		}
// 		ft_printf("Commande : %s\n", current->args[0]);
// 		i = 1;
// 		while (current->args[i])
// 		{
// 			ft_printf("Arg[%d]: %s\n", i, current->args[i]);
// 			i++;
// 		}
// 		if (current->infile)
// 		{
// 			if (current->append == -1)
// 				ft_printf("Lecture depuis un HEREDOC avec délimiteur : %s\n",
// 					current->infile);
// 			else
// 				ft_printf("Lecture depuis : %s\n", current->infile);
// 		}
// 		if (current->outfile)
// 			ft_printf("Écriture vers : %s (Append : %d)\n",
// 				current->outfile, current->append);
// 		if (current->next)
// 			ft_printf("⬇ Pipe vers la prochaine commande ⬇\n");
// 		current = current->next;
// 	}
// }


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

#include "minishell.h"

char	*get_token_type_name(t_token_type type)
{
	if (type == WORD) return "WORD";
	if (type == PIPE) return "PIPE";
	if (type == REDIR_IN) return "REDIR_IN";
	if (type == REDIR_OUT) return "REDIR_OUT";
	if (type == APPEND) return "APPEND";
	if (type == HEREDOC) return "HEREDOC";
	if (type == ENV_VAR) return "ENV_VAR";
	if (type == QUOTE) return "QUOTE";
	if (type == T_SPACE) return "T_SPACE";
	if (type == DELIMITER) return "DELIMITER";
	return "UNKNOWN";
}

void	print_tokens_debug(t_token *tokens)
{
	while (tokens)
	{
		printf("[%-10s] -> \"%s\"\n",
			get_token_type_name(tokens->type),
			tokens->value ? tokens->value : "(null)");
		tokens = tokens->next;
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
	term.c_lflag |= ISIG;        // permet à Ctrl-C et Ctrl-\ d’envoyer des signaux
	term.c_cc[VQUIT] = 28;       // remet Ctrl-\ (char code 28) comme touche de SIGQUIT
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
		// print_tokens_debug(data.tokens);
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
