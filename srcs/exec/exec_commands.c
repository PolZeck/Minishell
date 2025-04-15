/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:55:55 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/15 11:48:06 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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

extern int g_exit_status;

void execute_command(t_cmd *cmd, char **envp)
{
    pid_t   pid;
    char    *cmd_path;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		return;
	}
	
	// ft_printf("DEBUG: cmd->args[0] = '%s'\n", cmd->args[0]);
	// ft_printf("DEBUG arg[1] = '%s'\n", cmd->args[1]);

    if (is_builtin(cmd->args[0])) // Vérifie si c'est un builtin
    {
        execute_builtin(cmd); // Les builtins doivent eux aussi mettre à jour g_exit_status !
        return;
    }

    if (cmd->args[0][0] == '\0') // Si commande vide ""
    {
        ft_printf("bash: : command not found\n");
        g_exit_status = 127;
        return;
    }

    cmd_path = find_command_path(cmd->args[0]);
    if (!cmd_path)
    {
        ft_printf("bash: %s: command not found\n", cmd->args[0]);
        g_exit_status = 127;
        return;
    }

    pid = fork();
    if (pid == 0)
    {
        if (execve(cmd_path, cmd->args, envp) == -1)
        {
            perror("execve");
            exit(1);
        }
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            g_exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            g_exit_status = 128 + WTERMSIG(status);
    }

    free(cmd_path); // Libère le chemin de la commande
}
