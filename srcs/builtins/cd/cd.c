/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:20 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/19 14:16:35 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	cd_too_many_args(char **args, t_data *data)
{
	if (args[1] && args[2])
	{
		ft_putstr_fd("bash: cd: too many arguments\n", STDERR_FILENO);
		data->exit_status = 1;
		return (1);
	}
	return (0);
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
		data->exit_status = 1;
		free(old_copy);
		return (1);
	}
	data->exit_status = 0;
	update_env_pwd(data, old_copy);
	free(old_copy);
	return (0);
}

int	builtin_cd(t_cmd *cmd, t_data *data)
{
	char	*target;
	int		res;

	if (cd_too_many_args(cmd->args, data))
		return (1);
	target = get_cd_target(cmd->args, data);
	if (!target)
		return (1);
	res = cd_change_directory(target, data);
	free(target);
	return (res);
}
