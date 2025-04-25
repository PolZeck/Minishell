/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:39:00 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/25 15:58:19 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "minishell.h"

void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	setup_signals(void);
void	disable_ctrl_backslash(void);
void	setup_signals(void);
void	disable_ctrl_backslash(void);
void	enable_ctrl_backslash(void);

int		*get_exit_status(void);

#endif