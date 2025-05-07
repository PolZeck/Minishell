/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcosson <lcosson@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:41:07 by lcosson           #+#    #+#             */
/*   Updated: 2025/05/07 16:22:25 by lcosson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	init_pipes(t_pipex *pipex)
{
	if (pipe(pipex->pipe_fd) == -1)
	{
		clean(pipex);
		my_perr(ERR_PIPE, true);
	}
}
