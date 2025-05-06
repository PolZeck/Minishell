/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:52:18 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/06 14:55:43 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char	*get_home(t_data *data)
{
	char	*home;

	home = ft_getenv(data, "HOME");
	if (!home || !*home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		*get_exit_status() = 1;
		return (NULL);
	}
	return (home);
}
