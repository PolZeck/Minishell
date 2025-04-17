/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:02:20 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/17 15:06:40 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_cd(t_cmd *cmd)
{
	if (cmd->args[2])
	{
		ft_putstr_fd("bash: cd: too many arguments\n", STDERR_FILENO);
		*get_exit_status() = 1;
		return (1);
	}
	else if (cmd->args[1] == NULL)
	{
		ft_putstr_fd("minishell: cd: missing argument\n", STDERR_FILENO);
		*get_exit_status() = 1;
		return (1);
	}
	else if (chdir(cmd->args[1]) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(cmd->args[1]); // perror écrit déjà sur stderr
		*get_exit_status() = 1;
		return (1);
	}
	*get_exit_status() = 0;
	return (0);
}
