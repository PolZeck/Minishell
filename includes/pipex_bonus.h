/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:56:16 by lcosson           #+#    #+#             */
/*   Updated: 2025/04/24 15:27:04 by lcosson          ###   ########.fr       */
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

typedef struct s_cmd t_cmd;
typedef struct s_data t_data;


typedef struct s_pipex
{
	int		in_fd;
	int		out_fd;
	int		pipe_fd[2];
	int		prev_pipe_fd;
	int		num_cmds;
	pid_t	*pid;
	int		here_doc_flag;
	t_cmd 	*current_cmd; 
}			t_pipex;

# define TRUE 1
# define FALSE 0

# define HERE_DOC_FILE ".here_doc_tmp"

void	check_in_and_outfile_bonus(int argc, char **argv, t_pipex *pipex);
void	check_open_files_bonus(int argc, char **argv, t_pipex *pipex);
char	*check_addpath_cmd_bonus(char *cmd, char **envp, t_pipex *pipex);
char	*my_addpath_cmd(char *cmd, char *path, t_pipex *pipex);
char	*my_getpath(char **envp);

void	execute_cmd(t_cmd *cmd, char **envp, t_pipex *pipex);
pid_t	first_execution_bonus(int i, char **argv, char **envp, t_pipex *pipex);
pid_t	last_execution_bonus(int i, char **argv, char **envp, t_pipex *pipex);
pid_t	middle_execution_bonus(int i, char **argv, char **envp, t_pipex *pipex);

void	close_fds(t_pipex *pipex);
void	my_perr(const char *msg, bool should_exit);
void	my_free2(void **mallocated);
void	my_free(void *ptr);
void	clean(t_pipex *pipex);
void	handle_exec_error(char *path_cmd, char **tokenized_cmd);

void	init_struct(int argc, char **argv, t_pipex *pipex);
void	init_files(int argc, char **argv, t_pipex *pipex);
void	init_files_heredoc(int argc, char **argv, t_pipex *pipex);
void	init_pipes(t_pipex *pipex);
void	close_and_perr_dup2(t_pipex *pipex);
void	close_and_perr_fork(t_pipex *pipex);

void	write_here_doc(char *limiter, t_pipex *pipex);
int		my_pipex(char **argv, char **envp, t_pipex *pipex, t_cmd *cmd_list);
char	*get_last_file_of_type(t_list *redirs, int type1, int type2);

void	execute_pipex_builtin(char **args, char **envp, t_pipex *pipex);
int	execute_pipex_adapter(t_cmd *cmds, char **envp);
int	main_bonus(int argc, char **argv, char **envp);
bool check_redirections_failure(t_cmd *cmd);

#endif