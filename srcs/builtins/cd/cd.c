/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:20 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/09 15:33:39 by pledieu          ###   ########lyon.fr   */
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
	char	*target;

	home = NULL;
	target = NULL;
	if (!args[1] || args[1][0] == '~')
		home = get_home(data);
	if (!args[1])
	{
		if (!home)
			return (NULL);
		if (home[0] == '\0')
			return (ft_strdup("."));
		return (ft_strdup(home));
	}
	if (args[1][0] == '~' && args[1][1])
		target = ft_strjoin(home, args[1] + 1);
	else if (args[1][0] == '~')
		target = ft_strdup(home);
	else
		target = ft_strdup(args[1]);
	return (target);
}

static void	update_env_pwd(t_data *data, char *old_pwd)
{
	char	cwd[PATH_MAX];
	char	*new_pwd;
	char	*oldpwd;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		ft_putstr_fd("bash: chdir: error retrieving current directory:",
			STDERR_FILENO);
		ft_putstr_fd(
			" getcwd: cannot access parent directories",
			STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return ;
	}
	new_pwd = ft_strjoin("PWD=", cwd);
	data->env = replace_or_append_env(data->env, new_pwd);
	free(new_pwd);
	if (old_pwd)
	{
		oldpwd = ft_strjoin("OLDPWD=", old_pwd);
		data->env = replace_or_append_env(data->env, oldpwd);
		free(oldpwd);
	}
}

static int	cd_change_directory(char *target, t_data *data)
{
	char	*old_pwd;
	char	*old_copy;
	int		ret;

	old_pwd = ft_getenv(data, "PWD");
	old_copy = NULL;
	if (old_pwd)
		old_copy = ft_strdup(old_pwd);
	ret = chdir(target);
	if (ret != 0)
	{
		ft_putstr_fd("bash: cd: ", STDERR_FILENO);
		perror(target);
		*get_exit_status() = 1;
		free(old_copy);
		return (1);
	}
	*get_exit_status() = 0;
	update_env_pwd(data, old_copy);
	free(old_copy);
	return (0);
}

int	builtin_cd(t_cmd *cmd, t_data *data)
{
	char	*target;
	int		res;

	if (cd_too_many_args(cmd->args))
		return (1);
	target = get_cd_target(cmd->args, data);
	if (!target)
		return (1);
	res = cd_change_directory(target, data);
	free(target);
	return (res);
}
