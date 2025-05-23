/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:49:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/19 14:17:30 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

#include "builtins.h"
#include <errno.h>
#include <limits.h>

static int	handle_pwd_option(char *arg, t_data *data)
{
	if (arg && arg[0] == '-')
	{
		ft_putstr_fd("bash: pwd: ", 2);
		write(2, arg, 2);
		ft_putstr_fd(": invalid option\n", 2);
		data->exit_status = 2;
		return (0);
	}
	return (1);
}

int	builtin_pwd(t_cmd *cmd, t_data *data)
{
	char	*cwd;

	(void)data;
	if (!handle_pwd_option(cmd->args[1], data))
		return (2);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("bash: pwd: error retrieving current directory:",
			STDERR_FILENO);
		ft_putstr_fd(
			" getcwd: cannot access parent directories",
			STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		data->exit_status = 1;
		return (1);
	}
	ft_printf("%s\n", cwd);
	free(cwd);
	data->exit_status = 0;
	return (0);
}
