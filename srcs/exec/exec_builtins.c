/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:30:39 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/17 13:11:35 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipex_bonus.h"

void	execute_builtin(t_cmd *cmd,  t_data *data)
{
	if (!cmd->args[0])
		return ;
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		*get_exit_status() = builtin_cd(cmd);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		*get_exit_status() = builtin_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		*get_exit_status() = builtin_exit(cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		*get_exit_status() = builtin_pwd(cmd);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		*get_exit_status() = builtin_export(cmd, data);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		*get_exit_status() = builtin_unset(cmd, data);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		*get_exit_status() = builtin_env(cmd, data);
}

void	execute_pipex_builtin(char **args, char **envp, t_pipex *pipex)
{
	t_cmd	cmd;
	t_data	data;
	
	// Initialisation minimale des structures
	cmd.args = args;
	cmd.infile = NULL;
	cmd.outfile = NULL;
	cmd.next = NULL;
	
	data.env = envp; // Adaptation selon votre structure réelle
	
	// Utilisation de votre fonction existante
	execute_builtin(&cmd, &data);
	
	// Nettoyage et sortie
	clean(pipex);
	close_fds(pipex);
	exit(*get_exit_status()); // Utilise le statut défini par execute_builtin
}
