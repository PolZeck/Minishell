/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:39:00 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/06 15:14:08 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include "minishell.h"

extern volatile sig_atomic_t g_heredoc_interrupted;

void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	setup_signals(void);
void	heredoc_sigint_handler(int sig);
void	disable_ctrl_backslash(void);
void	enable_ctrl_backslash(void);

int		*get_exit_status(void);

#endif