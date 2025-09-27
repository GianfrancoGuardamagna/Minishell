NAME = pipex

PIPEX_SRCS = pipex.c utils/pipex_utils.c utils/pipex_split.c utils/pipex_aux.c
PIPEX_OBJS = $(PIPEX_SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(PIPEX_OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(PIPEX_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(PIPEX_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re