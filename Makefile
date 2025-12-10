# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gguardam <gguardam@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/06 12:00:00 by gguardam          #+#    #+#              #
#    Updated: 2025/12/10 12:43:55 by gguardam         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

SOURCES = main.c \
			user_route/dir_manager.c \
			user_route/dir_helpers.c \
			user_route/cd_handlers.c \
			commands_built/ft_echo.c \
			commands_built/ft_env.c \
			commands_built/ft_cd.c \
			commands_built/ft_pwd.c \
			commands_built/ft_exit.c \
			commands_built/ft_export.c \
			commands_built/ft_local_var.c \
			execute/execution_single_command.c \
			execute/execution_piped.c \
			execute/execution_piped_builtins.c \
			execute/execution_piped_child.c \
			utils/quotes_utils.c \
			utils/input_utils.c \
			utils/path_utils.c \
			utils/execution_piped_utils.c \
			utils/execution_utils.c \
			utils/binary_utils.c \
			utils/envs.c \
			utils/envs_utils.c \
			utils/local_vars_utils.c \
			utils/envs_export.c \
			utils/envs_helpers.c \
			utils/export_utils.c \
			utils/errors.c \
			utils/struct_utils.c \
			utils/input_validation.c \
			utils/cleanup_utils.c \
			utils/parsing_tokens_utils.c \
			utils/parsing_redirections_utils.c \
			utils/parsing_expander_utils.c \
			utils/expander_double_quotes_utils.c \
			signals/signals.c \
			parser/cleaner.c \
			parser/expander_double_quotes.c \
			parser/expander_helpers.c \
			parser/main_parsing.c \
			parser/parsing_args.c \
			parser/parsing_builtins.c \
			parser/parsing_expander.c \
			parser/parsing_init.c \
			parser/parsing_redirections.c \
			parser/parsing_tokens.c \
			parser/parsing_validator.c \

OBJECTS = $(SOURCES:.c=.o)

INCLUDES = -I. -I./libft
HEADERS = minishell.h

LIBS = -lreadline
LIBFT_DIR = libft.h
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_FLAGS = -L$(LIBFT_DIR) -lft

all: $(LIBFT) $(NAME)

$(LIBFT):
	@echo "Compiling libft..."
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJECTS) $(LIBFT)
	@echo "Linking $(NAME)..."
	$(CC) $(OBJECTS) $(LIBFT_FLAGS) $(LIBS) $(LDFLAGS) -o $(NAME)
	@echo "$(NAME) compiled successfully!"

%.o: %.c $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	rm -f $(OBJECTS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@echo "Cleaning executables..."
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re