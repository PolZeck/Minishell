/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:47:37 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/06 15:24:09 by lcosson          ###   ########.fr       */
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
int		is_valid_identifier_export(char *str);
int		var_exists(char **env, char *name);
char	**replace_env(char **env, char *new_entry, int idx);
char	**append_env(char **env, char *new_entry);
char	**replace_or_append_env(char **env, char *entry);

#endif