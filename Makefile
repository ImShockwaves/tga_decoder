.PHONY: all clean fclean re compile

COLOR_GREEN = "\033[1;32;40m"
COLOR_RED = "\033[1;31;40m"
COLOR_YELLOW = "\033[1;33;40m"
COLOR_BLUE = "\033[1;38;5;21m"
COLOR_ORANGE = "\033[1;38;5;202m"
COLOR_GRAY = "\033[1;38;5;244m"
COLOR_RESET = "\033[0m"

NAME = tga

CC = gcc
CFLAGS = -Wno-deprecated-declarations -Wall -Wextra -Werror
MLX_FLAGS = -framework OpenGL -framework GLUT

HEADER_PATH = includes

SRC_PATH = srcs
SRC_NAME = main.c

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))

OBJ_PATH = objs
OBJ_NAME = $(SRC_NAME:.c=.o)
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

all: compile

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) -c -o $@ $(CFLAGS) $< -I$(HEADER_PATH)

$(NAME): $(OBJ)
	@echo $(COLOR_YELLOW)"Linking "$@"..."$(COLOR_RESET)
	@echo $(COLOR_GRAY)
	$(CC) $^ $(MLX_FLAGS) -o $@
	@echo $(COLOR_RESET)
	@echo $(COLOR_GREEN)$(NAME)" successfully created."$(COLOR_RESET)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

compile: $(OBJ_PATH)
	@echo $(COLOR_YELLOW)"Compiling "$(NAME)"..."$(COLOR_RESET)
	@echo $(COLOR_GRAY)
	@make $(NAME)
	@echo $(COLOR_RESET)

clean:
	@echo $(COLOR_RED)"Removing "$(OBJ_PATH)"..."$(COLOR_RESET)
	rm -Rf $(OBJ_PATH)

fclean: clean
	@echo $(COLOR_RED)"Removing "$(NAME)"..."$(COLOR_RESET)
	rm -f $(NAME)

re: fclean all