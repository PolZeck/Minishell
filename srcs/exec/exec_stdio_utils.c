/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_stdio_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 00:52:04 by pol               #+#    #+#             */
/*   Updated: 2025/05/15 00:52:38 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	save_stdio(int *save_stdin, int *save_stdout)
{
	*save_stdin = dup(STDIN_FILENO);
	*save_stdout = dup(STDOUT_FILENO);
}

void	restore_stdio(int save_stdin, int save_stdout)
{
	if (save_stdin != -1)
	{
		dup2(save_stdin, STDIN_FILENO);
		close(save_stdin);
	}
	if (save_stdout != -1)
	{
		dup2(save_stdout, STDOUT_FILENO);
		close(save_stdout);
	}
}
