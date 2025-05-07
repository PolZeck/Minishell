/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:46:18 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/07 16:20:20 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"
# include "pipex_bonus.h"

typedef struct s_cmd	t_cmd;
typedef struct s_data	t_data;
typedef struct s_redir	t_redir;

void	execute_builtin(t_cmd *cmd, t_data *data);
void	execute_pipeline(t_cmd *cmd_list, t_data *data);
char	*find_command_path(char *cmd, t_data *data);
void	execute_command(t_cmd *cmd, t_data *data);
void	apply_redirections_in_child(t_cmd *cmd);
void	handle_child_output(t_redir *redir, int *out_fd);
int		validate_redirections(t_cmd *cmd);
void	print_err(char *prefix, char *cmd, char *message, int exit_code);

#endif