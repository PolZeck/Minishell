#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_token_type
{
    WORD,        // Commande ou argument
    PIPE,        // |
    REDIR_IN,    // <
    REDIR_OUT,   // >
    APPEND,      // >>
    HEREDOC,     // <<
    ENV_VAR,     // $VAR
    QUOTE,       // " ou '
    T_SPACE        // (On pourrait les ignorer après)
}   t_token_type;

typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token *next;
}   t_token;

typedef struct s_cmd
{
    char    **args;
    char    *infile;  // Pour la redirection <
    char    *outfile; // Pour la redirection >
    int     append;   // Mode APPEND si >>
    struct s_cmd *next;
}   t_cmd;

t_token *tokenize(char *input);
t_cmd   *parse_tokens(t_token *tokens);
void    free_tokens(t_token *tokens);
void    free_cmds(t_cmd *cmds);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char	*expand_env_var(char *token, int in_single_quotes);
int		check_unclosed_quotes(char *input);

// token_utils
int is_operator(char c);
t_token *create_token
		(char *value, t_token_type type, int in_single_quotes);
int check_unclosed_quotes(char *input);

//utils.c

t_cmd *create_cmd(void);

//parsing_utils.c
void	handle_argument(t_cmd *cmd, int *arg_count, size_t *args_size, char *value);
void	handle_pipe(t_cmd **cmd, int *arg_count, size_t *args_size);
void	handle_redirections(t_cmd *cmd, t_token **tokens);
void	handle_redir_in(t_cmd *cmd, t_token **tokens);
void	handle_redir_out(t_cmd *cmd, t_token **tokens, int append);
void	handle_heredoc(t_cmd *cmd, t_token **tokens);

#endif
