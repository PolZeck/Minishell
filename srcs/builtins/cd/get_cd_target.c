/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cd_target.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:35:53 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/15 15:37:15 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static int	handle_home_unset(char *home)
{
	if (!home)
	{
		*get_exit_status() = 1;
		return (1);
	}
	return (0);
}

static char	*handle_empty_args(char *home)
{
	if (home[0] == '\0')
		return (ft_strdup("."));
	return (ft_strdup(home));
}

static char	*build_target(char **args, char *home)
{
	if (args[1][0] == '~' && args[1][1])
		return (ft_strjoin(home, args[1] + 1));
	else if (args[1][0] == '~')
		return (ft_strdup(home));
	return (ft_strdup(args[1]));
}

char	*get_cd_target(char **args, t_data *data)
{
	char	*home;
	char	*target;

	home = NULL;
	target = NULL;
	if (!args[1] || args[1][0] == '~')
		home = get_home(data);
	if ((args[1] && args[1][0] == '~') || !args[1])
		if (handle_home_unset(home))
			return (NULL);
	if (!args[1])
		return (handle_empty_args(home));
	target = build_target(args, home);
	return (target);
}
