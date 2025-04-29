/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:20 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/29 16:45:14 by pledieu          ###   ########lyon.fr   */
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
		return (home);
	}
	return (args[1]);
}


static int	cd_change_directory(char *target)
{
	if (chdir(target) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(target);
		*get_exit_status() = 1;
		return (1);
	}
	*get_exit_status() = 0;
	return (0);
}

int	builtin_cd(t_cmd *cmd, t_data *data)
{
	char	*target;

	if (cd_too_many_args(cmd->args))
		return (1);
	target = get_cd_target(cmd->args, data);
	if (!target)
		return (1);
	return (cd_change_directory(target));
}

