# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gguardam <gguardam@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/06 12:00:00 by gguardam          #+#    #+#              #
#    Updated: 2025/10/03 13:48:00 by gguardam         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCDIR = .
SOURCES = main.c \
			user_route/dir_manager.c \
			commands_built/ft_echo.c \
			commands_built/ft_env.c \
			utils/envs.c \
			utils/errors.c \
			pipex/utils.c \
			pipex/aux.c \
			signals/signals.c \

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
	$(CC) $(OBJECTS) $(LIBFT_FLAGS) $(LIBS) -o $(NAME)
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