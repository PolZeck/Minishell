/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipex_adapter.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:50:45 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/17 16:39:03 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

#include "pipex_bonus.h" // contient les includes et définitions de t_cmd, etc.
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

static int	count_cmds(t_cmd *cmd)
{
	int count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return count;
}

static char	*join_args(char **args)
{
	char	*result;
	int		i;

	if (!args || !args[0])
		return (NULL);
	result = ft_strdup(args[0]);
	i = 1;
	while (args[i])
	{
		result = ft_strjoin(result, " "); // join + free prev
		result = ft_strjoin(result, args[i]);
		i++;
	}
	return result;
}

char	**build_fake_argv(t_cmd *cmds)
{
	int		nb_cmds = count_cmds(cmds);
	char	**argv = malloc(sizeof(char *) * (nb_cmds + 4));
	int		i = 0;
	t_cmd	*tmp = cmds;

	if (!argv)
		return NULL;
	argv[i++] = ft_strdup("minishell");

	// infile
	if (cmds->infile)
		argv[i++] = ft_strdup(cmds->infile);
	else
		argv[i++] = ft_strdup("/dev/stdin"); // fallback

	// commands
	while (tmp)
	{
		argv[i++] = join_args(tmp->args);
		tmp = tmp->next;
	}

	// outfile
	tmp = cmds;
	while (tmp->next)
		tmp = tmp->next;
	if (tmp->outfile)
		argv[i++] = ft_strdup(tmp->outfile);
	else
		argv[i++] = ft_strdup("/dev/stdout"); // fallback

	argv[i] = NULL;
	return argv;
}

int	execute_pipex_adapter(t_cmd *cmds, char **envp)
{
	char	**argv;
	int		argc;
	int		i;
	int		status = 0;

	argv = build_fake_argv(cmds);
	if (!argv)
	{
		perror("malloc");
		return (1);
	}
	for (argc = 0; argv[argc]; argc++) ;
	status = main_bonus(argc, argv, envp);
	// clean
	i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
	return (status);
}
