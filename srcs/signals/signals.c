/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:25:17 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/17 16:19:04 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


#include <termios.h>

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit_handler(int sig)
{
	(void)sig;
}

void	disable_ctrl_backslash(void)
{
	struct termios term;

	tcgetattr(STDIN_FILENO, &term); // Récupère les attributs du terminal
	term.c_cc[VQUIT] = _POSIX_VDISABLE; // Désactive `Ctrl-\`
	tcsetattr(STDIN_FILENO, TCSANOW, &term); // Applique les nouveaux attributs
}

void	setup_signals(void)
{
	signal(SIGINT, sigint_handler);  // ✅ Gérer Ctrl-C
	signal(SIGQUIT, sigquit_handler); // ✅ Empêcher Ctrl-\ de quitter le shell
	disable_ctrl_backslash(); // ✅ Désactiver complètement `Ctrl-\`
}
