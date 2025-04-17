/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:55:55 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/17 13:17:17 by lcosson          ###   ########.fr       */
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


void execute_command(t_cmd *cmd, t_data *data)
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
        execute_builtin(cmd, data); // Les builtins doivent eux aussi mettre à jour g_exit_status !
        return;
    }

    if (cmd->args[0][0] == '\0') // Si commande vide ""
    {
        ft_printf("bash: : command not found\n");
		*get_exit_status() = 127;
        return;
    }

    cmd_path = find_command_path(cmd->args[0]);
	if (!cmd_path)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		*get_exit_status() = 127;
		return;
	}


    pid = fork();
    if (pid == 0)
    {
        if (execve(cmd_path, cmd->args, data->env) == -1)
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
			*get_exit_status() = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
			*get_exit_status() = 128 + WTERMSIG(status);
    }

    free(cmd_path); // Libère le chemin de la commande
}
