/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:49:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/06 14:48:35 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_pwd(t_cmd *cmd, t_data *data)
{
	char	cwd[4096];

	(void)data;
	if (cmd->args[1] && cmd->args[1][0] == '-')
	{
		ft_putstr_fd("minishell: pwd: ", 2);
		write(2, cmd->args[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		*get_exit_status() = 2;
		return (2);
	}
	if (getcwd(cwd, sizeof(cwd)))
		printf("%s\n", cwd);
	else
		perror("pwd");
	*get_exit_status() = 0;
	return (0);
}
