/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:52:18 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/12 12:46:20 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char	*get_home(t_data *data)
{
	char	*home;

	home = ft_getenv(data, "HOME");
	if (!home)
	{
		ft_putstr_fd("bash: cd: HOME not set\n", STDERR_FILENO);
		*get_exit_status() = 1;
		return (NULL);
	}
	return (home);
}
