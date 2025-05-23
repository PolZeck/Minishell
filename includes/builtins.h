/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:47:37 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/19 14:19:18 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# include <limits.h>

typedef struct s_cmd	t_cmd;
typedef struct s_data	t_data;

int		builtin_cd(t_cmd *cmd, t_data *data);
char	*get_home(t_data *data);
int		builtin_echo(t_cmd *cmd);
int		builtin_exit(t_cmd *cmd, t_data *data);
int		builtin_pwd(t_cmd *cmd, t_data *data);
int		builtin_env(t_cmd *cmd, t_data *data);
int		builtin_unset(t_cmd *cmd, t_data *data);
int		builtin_export(t_cmd *cmd, t_data *data);
void	add_or_append_var(t_data *data, char *arg);
void	add_or_replace_var(t_data *data, char *arg);
int		is_valid_identifier_export(char *str);
int		handle_invalid_identifier(char *arg, t_data *data);
int		var_exists(char **env, char *name);
char	**replace_env(char **env, char *new_entry, int idx);
char	**append_env(char **env, char *new_entry);
char	**replace_or_append_env(char **env, char *entry);

int		has_too_many_args(char **args);
void	handle_invalid_argument(t_cmd *cmd, t_data *data);
void	handle_too_many_arguments(t_data *data);
int		str_to_llong(const char *str, long long *out);
int		is_strict_numeric(char *str);
void	print_numeric_error(char *arg, t_data *data);
void	exit_cleanup(t_cmd *cmd, t_data *data, int exit_code);
char	*get_cd_target(char **args, t_data *data);

#endif