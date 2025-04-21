/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipex_adapter.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:50:45 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/21 16:12:04 by pledieu          ###   ########lyon.fr   */
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

char	*get_last_file_of_type(t_list *redirs, int type1, int type2)
{
	t_redir	*redir;
	char	*last = NULL;

	while (redirs)
	{
		redir = (t_redir *)redirs->content;
		if (redir->type == type1 || redir->type == type2)
			last = redir->file;
		redirs = redirs->next;
	}
	return last;
}

char	**build_fake_argv(t_cmd *cmds)
{
	int		nb_cmds = count_cmds(cmds);
	char	**argv = malloc(sizeof(char *) * (nb_cmds + 4));
	int		i = 0;
	t_cmd	*tmp = cmds;
	char	*file;

	if (!argv)
		return NULL;
	argv[i++] = ft_strdup("minishell");

	// infile
	file = get_last_file_of_type(cmds->redirs, REDIR_IN, HEREDOC);
	if (file)
		argv[i++] = ft_strdup(file);
	else
		argv[i++] = ft_strdup("/dev/stdin");

	// commands
	while (tmp)
	{
		argv[i++] = join_args(tmp->args);
		tmp = tmp->next;
	}

	// outfile (dans la dernière commande)
	tmp = cmds;
	while (tmp->next)
		tmp = tmp->next;
	file = get_last_file_of_type(tmp->redirs, REDIR_OUT, APPEND);
	if (file)
		argv[i++] = ft_strdup(file);
	else
		argv[i++] = ft_strdup("/dev/stdout");

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
