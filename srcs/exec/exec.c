/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:29:12 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/27 16:06:57 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pipeline(t_cmd *cmd_list, char **envp)
{
	t_cmd	*cmd;
	int		count = 0;

	cmd = cmd_list;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}

	if (count == 1)
	{
		if (cmd_list->args && cmd_list->args[0])
			execute_command(cmd_list, envp);
	}
	else
	{
		ft_printf("🧪 Plusieurs commandes détectées, voici leur parsing :\n");
		cmd = cmd_list;
		int c = 1;
		while (cmd)
		{
			ft_printf("🔸 Commande %d : %s\n", c, cmd->args[0]);
			for (int i = 1; cmd->args[i]; i++)
				ft_printf("   Arg[%d]: %s\n", i, cmd->args[i]);
			cmd = cmd->next;
			c++;
		}
		ft_printf("➡️  Pas d'exécution car plusieurs commandes (pipes) — en attente de pipex.\n");
	}
}
