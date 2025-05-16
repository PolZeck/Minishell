/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 09:02:02 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/16 15:10:17 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "signals.h"
# include "parsing.h"
# include "pipex.h"
# include "exec.h"
# include "builtins.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <termios.h>
# include <limits.h>
# include <stdbool.h>

typedef struct s_cmd	t_cmd;
typedef struct s_data	t_data;

int	init_shell(t_data *data, char **envp, struct termios *term);
int	handle_null_input(t_data *data);
int	process_input(char *input, t_data *data);

#endif