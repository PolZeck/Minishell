# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/11 11:49:00 by pol               #+#    #+#              #
#    Updated: 2025/05/13 10:29:34 by pledieu          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

# === Désactivation des messages "Entering directory" ===
MAKEFLAGS += --no-print-directory

# === Nom de la librairie ===
NAME	= libft.a

# === Compilation ===
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -I../includes
DEPFLAGS = -MMD -MP
AR		= ar rcs

# === Dossiers ===
OBJ_DIR	= obj
DEP_DIR	= dep

# === Fichiers Sources ===
SRCS	= ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
		  ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c \
		  ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c \
		  ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c \
		  ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c \
		  ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c \
		  ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
		  ft_handlers.c ft_handlers2.c ft_parser.c ft_printf.c ft_utils.c \
		  ft_utils2.c get_next_line.c get_next_line_utils.c \
		  ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
		  ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
		  ft_lstmap.c ft_strcmp.c ft_strtok.c ft_strtok_r.c ft_strspn.c \
		  ft_strcspn.c ft_strjoin_char.c ft_split_charset.c

OBJS	= $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS	= $(patsubst %.c, $(DEP_DIR)/%.d, $(SRCS))

# === Compilation ===
all: $(NAME)
	@echo "✅ Libft compiled successfully!"

$(NAME): $(OBJS)
	@echo "🔹 Creating archive $(NAME)..."
	@$(AR) $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR) $(DEP_DIR)
	@echo "🔹 Compiling Libft: $<..."
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@ -MF $(patsubst %.c, $(DEP_DIR)/%.d, $<)

$(OBJ_DIR) $(DEP_DIR):
	@mkdir -p $@

# === Clean ===
clean:
	@echo "🧹 Cleaning Libft object files..."
	@rm -rf $(OBJ_DIR) $(DEP_DIR)

fclean: clean
	@echo "🧹 Removing $(NAME)..."
	@rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
