/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:49:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/21 13:28:14 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_pwd(t_cmd *cmd)
{
	char	*cwd;

	(void)cmd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("minishell: pwd: error retrieving current directory\n", 2);
		*get_exit_status() = 1;
		return (1);
	}

	ft_putstr_fd(cwd, 1);      // ✅ stdout
	ft_putstr_fd("\n", 1);     // newline séparé
	free(cwd);

	*get_exit_status() = 0;
	return (0);
}
