# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pledieu <pledieu@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/11 11:49:00 by pledieu               #+#    #+#              #
#    Updated: 2025/03/11 16:23:32 by pledieu          ###   ########lyon.fr    #
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

# === Couleurs ===
GREEN   = \033[0;32m
YELLOW  = \033[0;33m
CYAN    = \033[0;36m
RED     = \033[0;31m
NC      = \033[0m

# === Bibliothèques ===
LIBS	= -lreadline -lncurses

# === Dossiers ===
LIBFT_DIR	= libft
SRC_DIR		= srcs
PARSING_DIR	= $(SRC_DIR)/parsing
SIGNALS_DIR	= $(SRC_DIR)/signals
EXEC_DIR	= $(SRC_DIR)/exec
BUILTINS_DIR	= $(SRC_DIR)/builtins
OBJ_DIR		= obj
DEP_DIR		= dep
MSH_OBJ		= $(OBJ_DIR)/minishell
MSH_DEP		= $(DEP_DIR)/minishell

# === Fichier de suppression Valgrind ===
VALGRIND_SUPP = readline.supp

# === Fichiers Sources Minishell ===
MSH_SRCS	= main.c parsing/parsing.c parsing/tokenizer.c parsing/utils.c \
              parsing/tokenizer_utils.c parsing/parsing_utils.c parsing/utils_memory.c \
              signals/signals.c signals/signals_utils.c \
			  exec/exec_builtins.c exec/exec_pipes.c exec/exec_redirects.c exec/exec_utils.c exec/exec.c exec/exec_commands.c \
			  builtins/cd.c builtins/echo.c builtins/exit.c
MSH_OBJS	= $(patsubst %.c, $(MSH_OBJ)/%.o, $(MSH_SRCS))
MSH_DEPS	= $(patsubst %.c, $(MSH_DEP)/%.d, $(MSH_SRCS))

LIBFT_NAME	= $(LIBFT_DIR)/libft.a

# === Compilation Générale ===
all: libft $(NAME) $(VALGRIND_SUPP) banner

banner:
	@clear
	@echo "$(CYAN)\n\n\n\n"
	@echo "			███╗   ███╗ ██╗ ███╗   ██╗ ██╗ ███████╗ ██╗  ██╗ ███████╗ ██╗      ██╗     "
	@echo "			████╗ ████║ ██║ ████╗  ██║ ██║ ██╔════╝ ██║  ██║ ██╔════╝ ██║      ██║     "
	@echo "			██╔████╔██║ ██║ ██╔██╗ ██║ ██║ ███████╗ ███████║ █████╗   ██║      ██║     "
	@echo "			██║╚██╔╝██║ ██║ ██║╚██╗██║ ██║ ╚════██║ ██╔══██║ ██╔══╝   ██║      ██║     "
	@echo "			██║ ╚═╝ ██║ ██║ ██║ ╚████║ ██║ ███████║ ██║  ██║ ███████╗ ███████╗ ███████╗"
	@echo "			╚═╝     ╚═╝ ╚═╝ ╚═╝  ╚═══╝ ╚═╝ ╚══════╝ ╚═╝  ╚═╝ ╚══════╝ ╚══════╝ ╚══════╝"
	@echo "\n\n\n\n"
	@echo "$(NC)"

# Toujours exécuter `make -C libft` pour s'assurer que libft.a est à jour
libft:
	@echo "❗ Checking Libft..."
	@$(MAKE) -C $(LIBFT_DIR)

$(VALGRIND_SUPP):
	@echo "{\n   ignore_libreadline_leaks\n   Memcheck:Leak\n   ...\n   obj:*/libreadline.so*\n}" > $(VALGRIND_SUPP)
	@echo "✅ Fichier de suppression Valgrind créé : $(VALGRIND_SUPP)"

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
	@mkdir -p $(dir $@)
	@echo "🔹 Compiling $<..."
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@ -MF $(patsubst $(PARSING_DIR)/%.c, $(MSH_DEP)/parsing/%.d, $<)

$(MSH_OBJ)/signals/%.o: $(SIGNALS_DIR)/%.c | $(MSH_OBJ)/signals $(MSH_DEP)/signals
	@mkdir -p $(dir $@)
	@echo "🔹 Compiling $<..."
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@ -MF $(patsubst $(SIGNALS_DIR)/%.c, $(MSH_DEP)/signals/%.d, $<)

$(MSH_OBJ)/exec/%.o: $(EXEC_DIR)/%.c | $(MSH_OBJ)/exec $(MSH_DEP)/exec
	@mkdir -p $(dir $@)
	@echo "🔹 Compiling $<..."
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@ -MF $(patsubst $(EXEC_DIR)/%.c, $(MSH_DEP)/exec/%.d, $<)

$(MSH_OBJ)/exec/%.o: $(BUILTINS_DIR)/%.c | $(MSH_OBJ)/builtins $(MSH_DEP)/builtins
	@mkdir -p $(dir $@)
	@echo "🔹 Compiling $<..."
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@ -MF $(patsubst $(builtins_DIR)/%.c, $(MSH_DEP)/builtins/%.d, $<)

# === Création des dossiers nécessaires ===
$(MSH_OBJ):
	@mkdir -p $(MSH_OBJ)
	@mkdir -p $(MSH_OBJ)/parsing
	@mkdir -p $(MSH_OBJ)/signals
	@mkdir -p $(MSH_OBJ)/exec
	@mkdir -p $(MSH_OBJ)/builtins

$(MSH_DEP):
	@mkdir -p $(MSH_DEP)
	@mkdir -p $(MSH_DEP)/parsing
	@mkdir -p $(MSH_DEP)/signals
	@mkdir -p $(MSH_DEP)/exec
	@mkdir -p $(MSH_DEP)/builtins

# === Clean ===
clean:
	@echo "🧹 Cleaning Minishell object files..."
	@rm -rf $(OBJ_DIR) $(DEP_DIR) $(VALGRIND_SUPP)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "🧹 Removing $(NAME)..."
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(MSH_DEPS)

.PHONY: all clean fclean re libft
