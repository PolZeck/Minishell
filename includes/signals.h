/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:39:00 by pledieu           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/05/19 13:40:55 by lcosson          ###   ########.fr       */
=======
/*   Updated: 2025/05/19 14:30:09 by pol              ###   ########.fr       */
>>>>>>> leaks
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include "minishell.h"

extern volatile sig_atomic_t	g_heredoc_interrupted;

void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	setup_signals(void);
void	heredoc_sigint_handler(int sig);
void	disable_ctrl_backslash(void);
void	enable_ctrl_backslash(void);
<<<<<<< HEAD
void	handle_sigpipe(int sig);
int		*get_exit_status(void);
=======
>>>>>>> leaks

#endif