/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:52:18 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/19 13:34:33 by pol              ###   ########.fr       */
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
		data->exit_status = 1;
		return (NULL);
	}
	return (home);
}
