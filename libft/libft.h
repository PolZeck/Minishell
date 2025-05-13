/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 11:08:07 by pledieu           #+#    #+#             */
/*   Updated: 2025/05/13 10:29:20 by pledieu          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}				t_list;

t_list			*ft_lstnew(void *content);
t_list			*ft_lstlast(t_list *lst);
t_list			*ft_lstmap(t_list *lst, void *(*f)(void *),
					void (*del)(void *));

void			ft_lstadd_front(t_list **lst, t_list *new);
void			ft_lstdelone(t_list *lst, void (*del)(void*));
void			ft_lstadd_back(t_list **lst, t_list *new);
void			ft_lstclear(t_list **lst, void (*del)(void*));
void			ft_lstiter(t_list *lst, void (*f)(void *));
int				ft_lstsize(t_list *lst);

size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize);
unsigned int	ft_strlcat(char *dest, char *src, unsigned int size);

int				ft_isdigit(int c);
int				ft_isalpha(int c);
int				ft_isalnum(int c);
int				ft_atoi(const char *nptr);
int				ft_isprint(int c);
int				ft_isascii(int c);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_tolower(int c);
int				ft_toupper(int c);
int				ft_memcmp(const void *s1, const void *s2, size_t n);

void			*ft_memcpy(void *dest, const void *src, size_t n);
void			ft_bzero(void *s, size_t n);
void			*ft_memset(void *s, int c, size_t n);
void			*ft_memmove(void *dest, const void *src, size_t n);
void			*ft_memset(void *s, int c, size_t n);
void			*ft_memchr(const void *s, int c, size_t n);
void			*ft_calloc(size_t nmemb, size_t size);
void			ft_putnbr_fd(int n, int fd);
void			ft_putstr_fd(char *s, int fd);
void			ft_putchar_fd(char s, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_striteri(char *s, void (*f)(unsigned int, char*));

size_t			ft_strlen(const char *str);

char			*ft_strdup(const char *s);
char			**ft_split(char *str, char c);
char			**ft_split_charset(char *str, const char *charset);
char			*ft_itoa(int n);
char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
char			*ft_strnstr(const char *big, const char *little, size_t len);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strtrim(char const *s1, char const *set);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));

int				ft_printf(const char *format, ...);
int				ft_parse_format(char specifier, va_list args);
int				ft_handle_char(char c);
int				ft_handle_string(char *str);
int				ft_handle_int(int num);
int				ft_handle_hex(unsigned int num, int uppercase);
int				ft_handle_pointer(void *ptr);
int				ft_putchar(char c);
int				ft_putstr(char *str);
int				ft_handle_unsigned(unsigned int num);
int				ft_strcmp(char *s1, char *s2);

char			*ft_itoa_unsigned(unsigned int num);
char			*ft_itoa_gnl(int n);
char			*ft_itoa_base(unsigned long num, int base, int uppercase);
size_t			ft_strspn(const char *s, const char *accept);
size_t			ft_strcspn(const char *s, const char *reject);
char			*ft_strtok(char *str, const char *delim);
char			*ft_strtok_r(char *str, const char *delim, char **saveptr);
char			*ft_strjoin_char(const char *s1, const char *s2, char c);

char			*get_next_line(int fd);
char			*ft_strdup_gnl(const char *s1);
char			*ft_strjoin_gnl(char *s1, char *s2);
size_t			ft_strlen_gnl(const char *s);
int				ft_check_line(char *str);

#endif