/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:56:16 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/13 15:27:58 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "errors.h"
# include "../libft/libft.h"
# include "minishell.h"

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdbool.h>
# include <errno.h>

typedef struct s_cmd	t_cmd;
typedef struct s_data	t_data;
typedef struct s_redir	t_redir;

typedef struct s_pipex
{
	int		in_fd;
	int		out_fd;
	int		pipe_fd[2];
	int		prev_pipe_fd;
	int		num_cmds;
	pid_t	*pid;
	int		here_doc_flag;
	t_cmd	*current_cmd;
}			t_pipex;

# define TRUE 1
# define FALSE 0

# define HERE_DOC_FILE ".here_doc_tmp"

void	close_fds(t_pipex *pipex);
void	my_perr(const char *msg, bool should_exit);
void	my_free2(void **mallocated);
void	my_free(void *ptr);
void	clean(t_pipex *pipex);
void	handle_exec_error(char *path_cmd, char **tokenized_cmd);

void	execute_pipex_builtin(char **args, char **envp, t_pipex *pipex);
int		execute_pipex_direct(t_cmd *cmds, t_data *data);

bool	has_input_redir(t_cmd *cmd);
bool	has_output_redir(t_cmd *cmd);
void	apply_redirections(t_list *redirs);
int		wait_for_processes(t_pipex *pipex);

pid_t	first_execution_direct(t_pipex *pipex, t_data *data);
pid_t	middle_execution_direct(t_pipex *pipex, t_data *data);
pid_t	last_execution_direct(t_pipex *pipex, t_data *data);
void	open_and_close_all_outputs(t_list *redirs);
t_redir	*find_last_output_redirection(t_list *redirs);
void	run_builtin(t_cmd *cmd, t_data *data);

#endif