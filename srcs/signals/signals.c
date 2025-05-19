/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:25:17 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/19 14:43:01 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

volatile sig_atomic_t	g_heredoc_interrupted = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 2;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	(void)sig;
}

void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	signal(SIGPIPE, SIG_IGN);
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	close(STDIN_FILENO);
	write(STDOUT_FILENO, "\n", 1);
	rl_done = 1;
}

int	if_g_heredoc_interrupted(t_cmd *cmd, char *filename)
{
	if (g_heredoc_interrupted != 1)
		return (0);
	unlink(filename);
	free(filename);
	cmd->invalid = 1;
	g_heredoc_interrupted = 0;
	return (1);
}
