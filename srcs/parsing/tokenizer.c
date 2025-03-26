/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:19:52 by pledieu           #+#    #+#             */
/*   Updated: 2025/03/20 13:34:25 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*tokenize(char *input)
{
	t_token	*tokens = NULL;
	t_token	*last = NULL;
	int		i = 0;
	int		in_single_quotes = 0;
	int		start;
	char	*buffer;
	char	*temp;

	buffer = ft_strdup(""); // Initialise un buffer vide pour accumuler les expansions
	if (!buffer)
		return (NULL);
	while (input[i])
	{
		if (input[i] == ' ') // Ignorer les espaces
		{
			if (*buffer) // Si le buffer contient quelque chose, on en fait un token
			{
				t_token *new_token = create_token(buffer, WORD, in_single_quotes);
				if (!tokens)
					tokens = new_token;
				else
					last->next = new_token;
				last = new_token;
				free(buffer);
				buffer = ft_strdup(""); // Réinitialiser le buffer
			}
			i++;
			continue;
		}
		start = i;
		if (input[i] == '$' && !in_single_quotes) // Gestion des variables d’environnement
		{
			i++; // Passer le $
			start = i;
			while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
				i++;

			char *var_name = ft_substr(input, start, i - start);
			char *expanded = getenv(var_name);
			free(var_name);

			if (expanded)
			{
				temp = ft_strjoin(buffer, expanded); // On concatène la valeur de la variable au buffer
				free(buffer);
				buffer = temp;
			}
		}
		else if (input[i] == '"' || input[i] == '\'') // 🔥 Gestion des guillemets 🔥
		{
			char quote = input[i++]; // Stocke et avance après le guillemet ouvrant
			start = i; // Commence après le guillemet ouvrant
			while (input[i] && input[i] != quote) // Copie le texte sans guillemets
				i++;

			char *quoted_value = ft_substr(input, start, i - start); // Extrait sans les guillemets
			temp = ft_strjoin(buffer, quoted_value); // Concatène le texte extrait au buffer
			free(buffer);
			free(quoted_value);
			buffer = temp;

			if (input[i] == quote) // Ignore le guillemet fermant
				i++;
		}
		else // Autres caractères
		{
			while (input[i] && !is_operator(input[i]) && input[i] != ' ' && input[i] != '$')
				i++;

			char *word = ft_substr(input, start, i - start);
			temp = ft_strjoin(buffer, word); // Concatène le texte brut au buffer
			free(buffer);
			free(word);
			buffer = temp;
		}
	}
	if (*buffer)
	{
		t_token *new_token = create_token(buffer, WORD, in_single_quotes);
		if (!tokens)
			tokens = new_token;
		else
			last->next = new_token;
		free(buffer);
	}
	return tokens;
}

void	handle_token(t_token **tokens, t_token **last, char *input, int *i)
{
	char			*buffer;
	int				j;
	t_token_info	info;
	t_quote			q;

	buffer = malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!buffer)
		return ;
	j = 0;
	info.type = WORD;
	info.in_single_quotes = 0;
	q.buffer = buffer;
	q.input = input;
	q.i = i;
	q.j = &j;
	if (is_operator(input[*i]))
		info.type = handle_operator(buffer, input, i, &j);
	else
		process_word_or_quote(&q, &info);
	buffer[j] = '\0';
	info.buffer = buffer;
	add_token(tokens, last, info);
	free(buffer);
}

void	process_word_or_quote(t_quote *q, t_token_info *info)
{
	if (q->input[*q->i] == '"' || q->input[*q->i] == '\'')
		info->type = handle_quotes(q);
	else
	{
		while (q->input[*q->i] && !is_operator(q->input[*q->i])
			&& q->input[*q->i] != ' ')
			q->buffer[(*q->j)++] = q->input[(*q->i)++];
	}
}

t_token_type	handle_operator(char *buffer, char *input, int *i, int *j)
{
	t_token_type	type;

	buffer[(*j)++] = input[(*i)++];
	if (buffer[0] == '<' || buffer[0] == '>')
		type = handle_redirections_token(buffer, input, i, j);
	else
		type = PIPE;
	return (type);
}

void	handle_expansion(char *buffer, char *input, int *i, int *j)
{
	char	*var_name;
	int		start;
	int		k;
	char	*expanded;

	start = *i + 1;
	while (input[start] && (ft_isalnum(input[start]) || input[start] == '_'))
		start++;
	var_name = malloc(sizeof(char) * (start - *i));
	if (!var_name)
		return ;
	k = 0;
	(*i)++;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		var_name[k++] = input[(*i)++];
	var_name[k] = '\0';
	expanded = getenv(var_name);
	free(var_name);
	if (expanded)
	{
		k = 0;
		while (expanded[k])
			buffer[(*j)++] = expanded[k++];
	}
}

