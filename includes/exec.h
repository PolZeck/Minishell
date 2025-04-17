/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:46:18 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/17 13:13:47 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"
# include "pipex_bonus.h"

void	execute_builtin(t_cmd *cmd,  t_data *data);
void	execute_pipeline(t_cmd *cmd_list, t_data *data);
char	*find_command_path(char *cmd);
void	execute_command(t_cmd *cmd, t_data *data);

#endif