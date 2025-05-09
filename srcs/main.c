/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:43:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/09 11:33:21 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* void	debug_print_commands(t_cmd *cmd_list)
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
		printf("TOKEN[%d] = [%s], quote_type = %d\n",
		i, tokens->value, tokens->quote_type);
		tokens = tokens->next;
		i++;
	}
} */

/*
	fonction main, entrée du programme,
	gere qu'il n'ya bien pas d'input pour le ctrl + D
	puis tokenize et parse l'input,
	et enfin envoie execute_pipeline avec notre commande et l'envp
*/
#include "minishell.h"

static void	init_terminal_settings(struct termios *term)
{
	tcgetattr(STDIN_FILENO, term);
	term->c_lflag |= ISIG;
	term->c_cc[VQUIT] = 28;
	tcsetattr(STDIN_FILENO, TCSANOW, term);
}

static int	handle_null_input(t_data *data)
{
	write(1, "exit\n", 5);
	free_env(data->env);
	rl_clear_history();
	return (1);
}

static int	process_input(char *input, t_data *data)
{
	t_cmd	*cmd;

	add_history(input);
	if (check_unclosed_quotes(input))
	{
		perror("Erreur : guillemets non fermés");
		free(input);
		return (0);
	}
	data->tokens = tokenize(input, data);
	cmd = parse_tokens(data->tokens);
	data->cmds_head = cmd;
	if (cmd)
		execute_pipeline(cmd, data);
	free_tokens(data->tokens);
	free_cmds(cmd);
	free(input);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data			data;
	char			*input;
	struct termios	term;

	(void)argc;
	(void)argv;
	data.env = dup_env(envp, 1);
	if (!data.env)
		return (1);
	setup_signals();
	init_terminal_settings(&term);
	while (1)
	{
		disable_ctrl_backslash();
		input = readline("minishell> ");
		if (!input && handle_null_input(&data))
			break ;
		if (input && *input)
			process_input(input, &data);
		else
			free(input);
	}
	return (0);
}
