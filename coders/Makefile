NAME        = codexion
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread -I includes

SRC_DIR     = src
OBJ_DIR     = objs

SRCS        := $(shell find $(SRC_DIR) -name "*.c" ! -name "main.c")
MANDATORY   := $(SRCS) $(SRC_DIR)/main.c
OBJS        := $(MANDATORY:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# cores
GREEN       = \033[0;32m
CYAN        = \033[0;36m
YELLOW      = \033[0;33m
RED         = \033[0;31m
RESET       = \033[0m
BOLD        = \033[1m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(CYAN)🔗 Linking $(BOLD)$(NAME)$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✔ Build complete!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)⚙️  Compiling $(RESET)$<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)🧹 Cleaning objects$(RESET)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(RED)🗑  Removing binary$(RESET)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
