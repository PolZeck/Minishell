/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:20 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/05 13:25:20 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	cd_too_many_args(char **args)
{
	if (args[1] && args[2])
	{
		ft_putstr_fd("bash: cd: too many arguments\n", STDERR_FILENO);
		*get_exit_status() = 1;
		return (1);
	}
	return (0);
}


static char	*get_cd_target(char **args, t_data *data)
{
	char	*home;

	if (args[1] == NULL)
	{
		home = ft_getenv(data, "HOME");
		if (!home || !*home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			*get_exit_status() = 1;
			return (NULL);
		}
		return (ft_strdup(home));
	}
	if (args[1][0] == '~')
	{
		home = ft_getenv(data, "HOME");
		if (!home || !*home)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			*get_exit_status() = 1;
			return (NULL);
		}
		if (args[1][1] == '\0')
			return (ft_strdup(home));
		else if (args[1][1] == '/')
			return (ft_strjoin(home, args[1] + 1));
	}
	return (ft_strdup(args[1]));
}

static int	cd_change_directory(char *target, t_data *data)
{
	char	cwd[PATH_MAX];
	char	*old_pwd = NULL;
	char	*old_pwd_copy = NULL;
	char	*pwd_var;
	char	*oldpwd_var;

	old_pwd = ft_getenv(data, "PWD");
	if (old_pwd)
		old_pwd_copy = ft_strdup(old_pwd);
	if (chdir(target) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(target);
		*get_exit_status() = 1;
		free(old_pwd_copy);
		return (1);
	}
	*get_exit_status() = 0;

	if (getcwd(cwd, sizeof(cwd)))
	{
		pwd_var = ft_strjoin("PWD=", cwd);
		data->env = replace_or_append_env(data->env, pwd_var);
		free(pwd_var);
		if (old_pwd_copy)
		{
			oldpwd_var = ft_strjoin("OLDPWD=", old_pwd_copy);
			data->env = replace_or_append_env(data->env, oldpwd_var);
			free(oldpwd_var);
		}
	}
	free(old_pwd_copy);
	return (0);
}

int	builtin_cd(t_cmd *cmd, t_data *data)
{
	char	*target;
	int		result;

	if (cd_too_many_args(cmd->args))
		return (1);
	target = get_cd_target(cmd->args, data);
	if (!target)
		return (1);
	result = cd_change_directory(target, data);
	free(target);
	return (result);
}
