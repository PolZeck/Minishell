/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:37:53 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/23 12:19:50 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_env_var(char *token, t_quote_type quote_type)
{
	char	*env_value;

	// Pas d’expansion dans les quotes simples
	if (quote_type == SINGLE_QUOTE)
		return (ft_strdup(token));

	// Si ce n’est pas une variable
	if (!token || token[0] != '$')
		return (ft_strdup(token));

	// Variable spéciale `$?`
	if (ft_strcmp(token, "$?") == 0)
		return (ft_itoa(*get_exit_status()));

	// Cas "$" seul sans nom de variable
	if (token[1] == '\0')
		return (ft_strdup("$"));

	// Expansion normale
	env_value = getenv(token + 1);
	if (env_value)
		return (ft_strdup(env_value));
	return (ft_strdup(""));
}

int	count_args(t_token *tokens)
{
	int	count = 0;

	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == WORD || tokens->type == QUOTE)
			count++;
		else if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == HEREDOC || tokens->type == APPEND)
		{
			if (tokens->next)
				tokens = tokens->next; // skip redir target
		}
		tokens = tokens->next;
	}
	return (count);
}


t_cmd	*create_cmd(t_token *tokens)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);

	int arg_count = count_args(tokens);
	cmd->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}

	for (int i = 0; i <= arg_count; i++) // ✅ ici, on corrige le nom
		cmd->args[i] = NULL;

	cmd->redirs = NULL;
	cmd->invalid = 0;
	cmd->next = NULL;

	return (cmd);
}



int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

char	**dup_env(char **envp)
{
	int		i;
	char	**copy;

	i = 0;
	while (envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
