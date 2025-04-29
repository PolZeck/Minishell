/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:39:00 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/29 13:38:35 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include "minishell.h"

extern volatile sig_atomic_t heredoc_interrupted;

void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	setup_signals(void);
void	heredoc_sigint_handler(int sig);
void	disable_ctrl_backslash(void);
void	enable_ctrl_backslash(void);

int		*get_exit_status(void);

#endif