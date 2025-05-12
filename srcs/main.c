/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:43:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/12 12:45:35 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		perror("Error : quotes unclosed");
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
