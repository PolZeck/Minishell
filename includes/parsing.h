/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pol <pol@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:33:29 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/15 01:16:53 by pol              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"
# include <stdbool.h>

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
	T_SPACE,
	DELIMITER
}	t_token_type;

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_quote_type	quote_type;
	struct s_token	*next;
	bool			from_quotes;
}	t_token;

typedef struct s_token_info
{
	char			*buffer;
	t_token_type	type;
	int				in_single_quotes;
}	t_token_info;

typedef struct s_redir
{
	int		type;
	char	*file;
	int		fd;
	bool	expand;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_list			*redirs;
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
	t_token	*tokens;
	int		in_pipeline;
	t_cmd	*cmds_head;
	bool	from_quotes;
}	t_data;

typedef struct s_parseinfo
{
	t_data			*data;
	char			*input;
	int				*i;
	char			quote;
	t_quote_type	*quote_type;
	int				next_is_delimiter;
	bool			buffer_contains_quote;
	bool			came_from_quote;
	bool			actual_from_quotes;
}	t_parseinfo;

char			*ft_getenv(t_data *data, const char *name);

t_redir			*create_redir(int type, char *file);
void			add_redir(t_cmd *cmd, int type, char *file);

int				count_args(t_token *tokens);
int				check_unclosed_quotes(char *input);
int				is_builtin(char *cmd);
int				is_operator(char c);

char			*expand_env_var(char *token,
					t_quote_type quote_type, t_data *data);
char			**dup_env(char **envp, int initial);

t_cmd			*create_cmd(t_token *tokens);

void			syntax_error(char *token);
void			add_token(t_token **tokens, t_token **last,
					t_token_info info, t_data *data);
void			handle_argument(t_cmd *cmd, int *arg_count, char *value);
void			handle_pipe(t_cmd **cmd, int *arg_count,
					size_t *args_size, t_token **tokens);
void			handle_redir_in(t_cmd *cmd, t_token **tokens);
void			handle_redir_out(t_cmd *cmd, t_token **tokens, int append);
void			handle_heredoc(t_cmd *cmd, t_token **tokens, t_data	*data);
void			handle_expansion(char *buffer, char *input, int *i, int *j);
void			process_word_or_quote(t_quote *q, t_token_info *info);
void			flush_buffer_to_token(t_token_list token_list,
					char **buffer, t_quote_type quote_type, t_parseinfo *info);

void			handle_operator_token(t_token **tokens,
					t_token **last, t_parseinfo *info);
void			handle_quotes_in_token(char **buffer, t_parseinfo *info,
					t_token **tokens, t_token **last);
void			handle_variable_expansion(char **buffer,
					char *input, int *i, t_parseinfo *info);
void			append_word(char **buffer, char *input, int *i);

void			free_tokens(t_token *tokens);
void			free_cmds(t_cmd *cmds);
void			free_env(char **env);
void			*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void			free_split(char **split);

t_token			*tokenize(char *input, t_data *data);
t_token			*create_token(char *value, t_token_type type,
					t_quote_type quote_type, t_data *data);

t_token_type	handle_operator(char *buffer,
					char *input, int *i, int *j);
t_token_type	handle_quotes(t_quote *q);
t_token_type	get_token_type_from_op(char *op);
t_token_type	handle_redirections_token(char *buffer,
					char *input, int *i, int *j);
void			handle_input_token(t_token_list *tlist,
					char **buffer, t_parseinfo *info);

//parse_token
t_cmd	*parse_tokens(t_token *tokens, t_data *data);
bool	handle_token_content(t_cmd *cmd, t_token **tokens,
	int *arg_count, t_data	*data);
bool			should_split_token(t_token *token);
void			finalize_args(t_cmd *head);
bool	parse_all_tokens(t_token *tokens, t_cmd *cmd, t_cmd *head, t_data *data);
bool			handle_pipe_token(t_token **tokens, t_cmd **cmd,
					int *arg_count, t_cmd *head);

//dup_env

int				count_env(char **envp);
char			*increment_shlvl(const char *shlvl_entry);

//tokenizer
void			init_parseinfo(t_parseinfo *info, char *input,
					t_data *data, int *i);
void			init_token_structs(t_token_list *tlist);

int				if_g_heredoc_interrupted(t_cmd *cmd, char *filename);
void			generate_random_name(char *output, size_t len);
void			handle_variable_expansion_tokenizer(char **buffer,
					t_parseinfo *info);

#endif