# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/11 11:49:00 by pol               #+#    #+#              #
#    Updated: 2025/03/11 16:19:05 by pledieu          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

# === Désactivation des messages "Entering directory" ===
MAKEFLAGS += --no-print-directory

# === Nom du programme final ===
NAME	= minishell

# === Compilation ===
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -Iincludes -Ilibft
DEPFLAGS = -MMD -MP
AR		= ar rcs

# === Bibliothèques ===
LIBS	= -lreadline -lncurses

# === Dossiers ===
LIBFT_DIR	= libft
SRC_DIR		= srcs
PARSING_DIR	= $(SRC_DIR)/parsing
OBJ_DIR		= obj
DEP_DIR		= dep
MSH_OBJ		= $(OBJ_DIR)/minishell
MSH_DEP		= $(DEP_DIR)/minishell

# === Fichiers Sources Minishell ===
MSH_SRCS	= main.c parsing/parsing.c parsing/tokenizer.c parsing/utils.c parsing/tokenizer_utils.c parsing/parsing_utils.c
MSH_OBJS	= $(patsubst %.c, $(MSH_OBJ)/%.o, $(MSH_SRCS))
MSH_DEPS	= $(patsubst %.c, $(MSH_DEP)/%.d, $(MSH_SRCS))

LIBFT_NAME	= $(LIBFT_DIR)/libft.a

# === Compilation Générale ===
all: libft $(NAME)
	@echo "✅ Build complete!"

# Toujours exécuter `make -C libft` pour s'assurer que libft.a est à jour
libft:
	@echo "❗ Checking Libft..."
	@$(MAKE) -C $(LIBFT_DIR)

# Relink Minishell si libft.a a changé
$(NAME): $(MSH_OBJS) $(LIBFT_NAME)
	@echo "🔹 Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(MSH_OBJS) -L$(LIBFT_DIR) -lft $(LIBS) -o $(NAME)

# === Compilation de Minishell ===
$(MSH_OBJ)/%.o: $(SRC_DIR)/%.c | $(MSH_OBJ) $(MSH_DEP)
	@mkdir -p $(dir $@)  # Crée dynamiquement le dossier où doit aller le .o
	@echo "🔹 Compiling $<..."
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@ -MF $(patsubst $(SRC_DIR)/%.c, $(MSH_DEP)/%.d, $<)

# === Compilation des fichiers dans srcs/parsing ===
$(MSH_OBJ)/parsing/%.o: $(PARSING_DIR)/%.c | $(MSH_OBJ)/parsing $(MSH_DEP)/parsing
	@mkdir -p $(dir $@)  # Crée dynamiquement le dossier où doit aller le .o
	@echo "🔹 Compiling $<..."
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@ -MF $(patsubst $(PARSING_DIR)/%.c, $(MSH_DEP)/parsing/%.d, $<)

# === Création des dossiers nécessaires ===
$(MSH_OBJ):
	@mkdir -p $(MSH_OBJ)
	@mkdir -p $(MSH_OBJ)/parsing  # Création du sous-dossier parsing

$(MSH_DEP):
	@mkdir -p $(MSH_DEP)
	@mkdir -p $(MSH_DEP)/parsing  # Création du sous-dossier parsing pour les dépendances

# === Clean ===
clean:
	@echo "🧹 Cleaning Minishell object files..."
	@rm -rf $(OBJ_DIR) $(DEP_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "🧹 Removing $(NAME)..."
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(MSH_DEPS)

.PHONY: all clean fclean re libft
