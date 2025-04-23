/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:33:29 by pledieu           #+#    #+#             */
/*   Updated: 2025/04/22 15:33:19 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"


typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	ENV_VAR,
	QUOTE,
	T_SPACE
}	t_token_type;

typedef enum e_quote_type {
    NO_QUOTE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
}	t_quote_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_quote_type  quote_type;
	struct s_token	*next;
}	t_token;

typedef struct s_token_info
{
	char			*buffer;
	t_token_type	type;
	int				in_single_quotes;
}	t_token_info;

typedef struct s_redir
{
	int		type;  // REDIR_IN, REDIR_OUT, APPEND, HEREDOC
	char	*file;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_list			*redirs;  // 🔥 Nouvelle liste de redirections
	int				invalid;
	struct s_cmd	*next;
}	t_cmd;


typedef struct s_quote
{
	char	*buffer;
	char	*input;
	int		*i;
	int		*j;
	int		in_single_quotes;
}	t_quote;

typedef struct s_token_list
{
	t_token	**tokens;
	t_token	**last;
}	t_token_list;

typedef struct s_data
{
	char	**env;
}	t_data;

t_redir	*create_redir(int type, char *file);
void	add_redir(t_cmd *cmd, int type, char *file);

int	count_args(t_token *tokens);

int				check_unclosed_quotes(char *input);
int				is_builtin(char *cmd);
int				is_operator(char c);
int				check_unclosed_quotes(char *input);

char	*expand_env_var(char *token, t_quote_type quote_type);
char			**dup_env(char **envp);


t_cmd			*parse_tokens(t_token *tokens);
t_cmd	*create_cmd(t_token *tokens);

void			add_token(t_token **tokens, t_token **last, t_token_info info);
void			handle_argument(t_cmd *cmd,
					int *arg_count, size_t *args_size, char *value);
void			handle_pipe(t_cmd **cmd, int *arg_count, size_t *args_size, t_token **tokens);
void			handle_redir_in(t_cmd *cmd, t_token **tokens);
void			handle_redir_out(t_cmd *cmd, t_token **tokens, int append);
void			handle_heredoc(t_cmd *cmd, t_token **tokens);
void			handle_token(t_token **tokens,
			t_token **last, char *input, int *i);
void			handle_expansion(char *buffer, char *input, int *i, int *j);
void			process_word_or_quote(t_quote *q, t_token_info *info);
void	flush_buffer_to_token(t_token **tokens, t_token **last, char **buffer, t_quote_type quote_type);
void			handle_operator_token(t_token **tokens, t_token **last, char *input, int *i);
void	handle_quotes_in_token(char **buffer, char *input, int *i, t_quote_type *quote_type);
void			handle_variable_expansion(char **buffer, char *input, int *i);
void			append_word(char **buffer, char *input, int *i);
void			free_tokens(t_token *tokens);
void			free_cmds(t_cmd *cmds);
void	free_env(char **env);
void			*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void	free_split(char **split);

t_token			*tokenize(char *input);
t_token	*create_token(char *value, t_token_type type, t_quote_type quote_type);


t_token_type	handle_operator(char *buffer, char *input, int *i, int *j);
t_token_type	handle_quotes(t_quote *q);
t_token_type	get_token_type_from_op(char *op);
t_token_type	handle_redirections_token(char *buffer,
					char *input, int *i, int *j);

#endif