/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:55:55 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/20 11:24:18 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

char	*find_command_path(char *cmd)
{
	char	*path;
	char	**paths;
	char	*full_path;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
	path = getenv("PATH"); // Récupérer la variable PATH
	if (!path)
		return (NULL);
	paths = ft_split(path, ':'); // Diviser les chemins par ':'
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			while (paths[i]) // Libérer `split()`
				free(paths[i++]);
			free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	i = 0;
	while (paths[i]) // Libérer `split()`
		free(paths[i++]);
	free(paths);
	return (NULL);
}


void execute_command(t_cmd *cmd, char **envp)
{
    pid_t pid;
    char *cmd_path;

    if (!cmd || !cmd->args[0])
        return;

    if (is_builtin(cmd->args[0])) // Vérifie si c'est un builtin
    {
        execute_builtin(cmd); // Cette fonction gérera les builtins
        return;
    }

    // Pour les commandes externes
    cmd_path = find_command_path(cmd->args[0]);
    if (!cmd_path)
    {
        ft_printf("bash: %s: command not found\n", cmd->args[0]);
        return;
    }

    pid = fork();
    if (pid == 0) // Processus enfant
    {
        if (execve(cmd_path, cmd->args, envp) == -1)
            perror("execve");
        exit(127);
    }
    else if (pid > 0) // Processus parent
        waitpid(pid, NULL, 0);

    free(cmd_path); // Libère le chemin de la commande
}
