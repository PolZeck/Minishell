/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:47:37 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/15 15:50:37 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

// Builtins : CD
int	builtin_cd(t_cmd *cmd);

// Builtins : ECHO
int	builtin_echo(t_cmd *cmd);

// Builtins : EXIT
int	builtin_exit(t_cmd *cmd);
int	builtin_pwd(t_cmd *cmd);
#endif