NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDES = -I Codexion

SRC = Codexion/main.c \
      Codexion/parsing.c \
      Codexion/initialization.c \
      Codexion/time_utils.c \
      Codexion/scheduler.c \
      Codexion/coder_routine.c \
      Codexion/burnout.c \
      Codexion/threads.c

OBJ_DIR = obj

OBJ = $(SRC:Codexion/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: Codexion/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
