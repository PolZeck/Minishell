# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/11 11:49:00 by pol               #+#    #+#              #
#    Updated: 2025/03/07 12:18:55 by pledieu          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

# === Nom du programme final ===
NAME	= minishell

# === Compilation ===
DEBUG	= 0
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -Iincludes
DEPFLAGS = -MMD -MP
AR		= ar rcs

# === Dossiers ===
LIBFT_DIR	= libft
SRC_DIR		= srcs
OBJ_DIR		= obj
DEP_DIR		= dep
LIBFT_OBJ	= $(OBJ_DIR)/libft
LIBFT_DEP	= $(DEP_DIR)/libft
MSH_OBJ		= $(OBJ_DIR)/minishell
MSH_DEP		= $(DEP_DIR)/minishell

# === Fichiers Libft ===
LIBFT_SRCS	= libft/ft_atoi.c libft/ft_bzero.c libft/ft_calloc.c libft/ft_isalnum.c libft/ft_isalpha.c \
		  libft/ft_isascii.c libft/ft_isdigit.c libft/ft_isprint.c libft/ft_itoa.c libft/ft_memchr.c \
		  libft/ft_memcmp.c libft/ft_memcpy.c libft/ft_memmove.c libft/ft_memset.c libft/ft_putchar_fd.c \
		  libft/ft_putendl_fd.c libft/ft_putnbr_fd.c libft/ft_putstr_fd.c libft/ft_split.c \
		  libft/ft_strchr.c libft/ft_strdup.c libft/ft_striteri.c libft/ft_strjoin.c libft/ft_strlcat.c \
		  libft/ft_strlcpy.c libft/ft_strlen.c libft/ft_strmapi.c libft/ft_strncmp.c libft/ft_strnstr.c \
		  libft/ft_strrchr.c libft/ft_strtrim.c libft/ft_substr.c libft/ft_tolower.c libft/ft_toupper.c \
		  libft/ft_handlers.c libft/ft_handlers2.c libft/ft_parser.c libft/ft_printf.c libft/ft_utils.c \
		  libft/ft_utils2.c libft/get_next_line.c libft/get_next_line_utils.c \
		  libft/ft_lstnew.c libft/ft_lstadd_front.c libft/ft_lstsize.c libft/ft_lstlast.c \
		  libft/ft_lstadd_back.c libft/ft_lstdelone.c libft/ft_lstclear.c libft/ft_lstiter.c \
		  libft/ft_lstmap.c

LIBFT_OBJS	= $(patsubst libft/%.c, $(LIBFT_OBJ)/%.o, $(LIBFT_SRCS))
LIBFT_DEPS	= $(patsubst libft/%.c, $(LIBFT_DEP)/%.d, $(LIBFT_SRCS))

LIBFT_NAME	= $(LIBFT_DIR)/libft.a

# === Fichiers Minishell ===
MSH_SRCS	= srcs/main.c 
MSH_OBJS	= $(patsubst srcs/%.c, $(MSH_OBJ)/%.o, $(MSH_SRCS))
MSH_DEPS	= $(patsubst srcs/%.c, $(MSH_DEP)/%.d, $(MSH_SRCS))

# === Compilation avec ou sans Debug ===
ifeq ($(DEBUG), 1)
    CFLAGS += -g
endif

# === Compilation générale ===
all: $(NAME)

$(NAME): $(LIBFT_NAME) $(MSH_OBJS)
	@echo "Compiling $(NAME)..."
	$(CC) $(CFLAGS) $(MSH_OBJS) -L$(LIBFT_DIR) -lft -o $(NAME)

# === Compilation de la Libft ===
$(LIBFT_NAME): $(LIBFT_OBJS)
	@echo "Building $(LIBFT_NAME)..."
	$(AR) $(LIBFT_NAME) $(LIBFT_OBJS)

$(LIBFT_OBJ)/%.o: libft/%.c | $(LIBFT_OBJ) $(LIBFT_DEP)
	@echo "Compiling Libft: $<..."
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@ -MF $(LIBFT_DEP)/$*.d

# === Compilation de Minishell ===
$(MSH_OBJ)/%.o: srcs/%.c | $(MSH_OBJ) $(MSH_DEP)
	@echo "Compiling Minishell: $<..."
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@ -MF $(MSH_DEP)/$*.d

# === Création des dossiers ===
$(LIBFT_OBJ) $(LIBFT_DEP) $(MSH_OBJ) $(MSH_DEP):
	@mkdir -p $@

# === Clean ===
clean:
	@echo "Cleaning object files..."
	rm -rf $(OBJ_DIR) $(DEP_DIR)

fclean: clean
	@echo "Removing $(NAME) and $(LIBFT_NAME)..."
	rm -f $(NAME) $(LIBFT_NAME)

re: fclean all

-include $(LIBFT_DEPS) $(MSH_DEPS)

.PHONY: all clean fclean re
