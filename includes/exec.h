/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:46:18 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/05 12:25:38 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"
# include "pipex_bonus.h"

typedef struct s_cmd	t_cmd;
typedef struct s_data	t_data;

void	execute_builtin(t_cmd *cmd, t_data *data);
void	execute_pipeline(t_cmd *cmd_list, t_data *data);
char	*find_command_path(char *cmd, t_data *data);
void	execute_command(t_cmd *cmd, t_data *data);

#endif