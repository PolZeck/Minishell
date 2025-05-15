/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:43:36 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/15 15:47:57 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_data			data;
	char			*input;
	struct termios	term;

	(void)argc;
	(void)argv;
	if (init_shell(&data, envp, &term))
		return (1);
	while (1)
	{
		g_heredoc_interrupted = 0;
		disable_ctrl_backslash();
		input = readline("minishell> ");
		if (!input && handle_null_input(&data))
			break ;
		if (input && *input)
			process_input(input, &data);
		else
			free(input);
	}
	return (0);
}
