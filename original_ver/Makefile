PIPEX_DIR = .
PRINTF_DIR = $(PIPEX_DIR)/submodules/printf
GNL_DIR = $(PIPEX_DIR)/submodules/get_next_line

CC				= cc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror

AR				= ar rcs

NAME = pipex
PRINTF = $(PRINTF_DIR)/libftprintf.a

SRCS = \
	$(addprefix $(PIPEX_DIR)/, \
		main.c \
		pipex.c \
		cmd_parse.c \
		here_doc.c \
		utils.c \
		execute.c \
	) \
	$(addprefix $(GNL_DIR)/, \
		get_next_line_bonus.c \
		get_next_line_utils_bonus.c \
	)

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(PRINTF):
	@make -C $(PRINTF_DIR)

$(NAME): $(OBJS) $(PRINTF)
	$(CC) $(OBJS) $(PRINTF) -I$(PRINTF_DIR) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I$(PRINTF_DIR) -O3 -c $< -o $@

clean:
	$(RM) $(OBJS)
	@make -C $(PRINTF_DIR) clean

fclean:	clean
	$(RM) $(NAME)
	@make -C $(PRINTF_DIR) fclean

re: fclean all

.PHONY:	all clean fclean re
