NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDES = -I src

SRC = src/main.c \
      src/parsing.c \
      src/initialization.c \
      src/time_utils.c \
      src/scheduler.c \
      src/coder_routine.c \
      src/burnout.c \
      src/threads.c

OBJ_DIR = obj

OBJ = $(SRC:src/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
