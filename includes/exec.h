/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:46:18 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/19 14:15:25 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"
# include "pipex.h"

typedef struct s_cmd	t_cmd;
typedef struct s_data	t_data;
typedef struct s_redir	t_redir;
typedef struct s_pipex	t_pipex;

char	*find_command_path(char *cmd, t_data *data);
int		validate_redirections(t_cmd *cmd);
void	execute_builtin(t_cmd *cmd, t_data *data);
void	execute_pipeline(t_cmd *cmd_list, t_data *data);
void	execute_command(t_cmd *cmd, t_data *data);
void	apply_redirections_in_child(t_cmd *cmd);
void	handle_child_output(t_redir *redir, int *out_fd);
void	print_err(char *prefix, char *cmd, char *message, int exit_code, t_data *data);
void	execute_command_and_exit(t_pipex *pipex, t_data *data);
void	execute_command(t_cmd *cmd, t_data *data);
void	save_stdio(int *save_stdin, int *save_stdout);
void	restore_stdio(int save_stdin, int save_stdout);

int		precheck_command(t_cmd *cmd, t_data *data);
int		apply_redirs(t_list *redirs, t_data *data);
char	*resolve_cmd_path(t_cmd *cmd, t_data *data);
void	run_child(t_cmd *cmd, t_data *data, char *path);
void	wait_and_handle(pid_t pid, int saved_stdout, t_data *data);
int		run_exit_exception(t_cmd *cmd, t_data *data);

#endif