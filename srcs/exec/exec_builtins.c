/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:30:39 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/17 14:51:04 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipex_bonus.h"

void	execute_builtin(t_cmd *cmd, t_data *data)
{
	int		save_stdin = -1;
	int		save_stdout = -1;
	int		fd;

	// 🔁 Appliquer redirection entrée
	if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd == -1)
		{
			perror(cmd->infile);
			*get_exit_status() = 1;
			return;
		}
		save_stdin = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}

	// 🔁 Appliquer redirection sortie
	if (cmd->outfile)
	{
		if (cmd->append)
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror(cmd->outfile);
			*get_exit_status() = 1;
			return;
		}
		save_stdout = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}

	// ✅ Lancement des builtins comme avant
	if (!cmd->args[0])
		return;
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

	// 🔁 Rétablir les descripteurs originaux
	if (save_stdin != -1)
	{
		dup2(save_stdin, STDIN_FILENO);
		close(save_stdin);
	}
	if (save_stdout != -1)
	{
		dup2(save_stdout, STDOUT_FILENO);
		close(save_stdout);
	}
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
