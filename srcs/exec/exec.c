/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:29:12 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/18 15:00:48 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_pipeline(t_cmd *cmd_list, char **envp)
{
	t_cmd	*cmd;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->args[0])
			execute_command(cmd, envp); // Exécuter chaque commande
		cmd = cmd->next;
	}
}


void	execute_single_command(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	char	*full_path;

	if (is_builtin(cmd->args[0])) // ✅ Vérifie si c'est un builtin
	{
		execute_builtin(cmd);
		return ;
	}

	full_path = find_command_path(cmd->args[0]); // ✅ Trouver `/bin/ls`
	if (!full_path)
	{
		ft_printf("minishell: %s: command not found\n", cmd->args[0]);
		return ;
	}

	pid = fork();
	if (pid == 0) // Enfant
	{
		handle_redirections_exec(cmd); // ✅ Appliquer les redirections `<`, `>`, `>>`
		if (execve(full_path, cmd->args, envp) == -1) // ✅ Utilisation correcte de `envp`
			perror("execve");
		exit(127);
	}
	else if (pid > 0) // Parent
		waitpid(pid, NULL, 0);
}
