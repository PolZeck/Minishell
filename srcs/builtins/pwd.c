/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:49:31 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/29 16:39:47 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_pwd(t_cmd *cmd, t_data *data)
{
	char	cwd[4096];

	(void)data;
	// gestion des options invalides
	if (cmd->args[1] && cmd->args[1][0] == '-')
	{
		ft_putstr_fd("minishell: pwd: ", 2);
		write(2, cmd->args[1], 2); // on affiche juste les 2 premiers caractères (comme bash)
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
