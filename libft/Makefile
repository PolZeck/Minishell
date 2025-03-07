# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/11 11:49:00 by pol               #+#    #+#              #
#    Updated: 2025/03/07 11:41:17 by pledieu          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME	= libft.a

DEBUG	= 0

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

SRCS	= ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c\
		ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c\
		ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c\
		ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c\
		ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c\
		ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c\
		ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c\
		ft_handlers.c ft_handlers2.c ft_parser.c ft_printf.c ft_utils.c\
		ft_utils2.c get_next_line.c get_next_line_utils.c\
		ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c\
    	ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c\
        ft_lstmap.c
	
OBJS	= $(SRCS:.c=.o)

ifeq ($(DEBUG), 1)
    CFLAGS += -g
endif

all: $(NAME)

$(NAME): $(OBJS)
	ar -rcs $(NAME) $(OBJS)

bonus: $(OBJS) $(B_OBJS)
	ar -rcs $(NAME) $(OBJS) $(B_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(B_OBJS)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all bonus clean fclean re
