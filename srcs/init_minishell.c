/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:48:06 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/19 15:38:43 by pol              ###   ########.fr       */
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

void	handle_sigint_status(t_data *data)
{
	if (g_heredoc_interrupted == 2)
	{
		data->exit_status = 130;
		g_heredoc_interrupted = 0;
	}
}

int	handle_null_input(t_data *data)
{
	write(1, "exit\n", 5);
	free_env(data->env);
	rl_clear_history();
	return (1);
}

int	process_input(char *input, t_data *data)
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
	cmd = parse_tokens(data->tokens, data);
	data->cmds_head = cmd;
	if (cmd)
		execute_pipeline(cmd, data);
	free_tokens(data->tokens);
	free_cmds(cmd);
	free(input);
	return (0);
}

int	init_shell(t_data *data, char **envp, struct termios *term)
{
 	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
	{
		ft_putendl_fd("minishell: not interactive mode", STDERR_FILENO);
		return (1);
	}
	data->exit_status = 0;
	data->env = dup_env(envp, 1);
	if (!data->env)
		return (1);
	setup_signals();
	init_terminal_settings(term);
	return (0);
}
