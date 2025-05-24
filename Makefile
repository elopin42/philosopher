NAME = philo
FLAGS = -Wall -Werror -Wextra -I inc/ -g3 #-fsanitize=thread

SRCSDIR = srcs/
OBJSDIR = objs/

SRCSF = main.c	\
		init.c	\
		tools_libft.c	\
		routine.c	\
		clean_up.c
SRCS = $(addprefix $(SRCSDIR), $(SRCSF))
OBJSF = $(SRCSF:.c=.o)
OBJS = $(addprefix $(OBJSDIR), $(OBJSF))

all: $(NAME)

$(NAME): $(OBJSDIR) $(OBJS)
	@echo "\033[0m\033[1;35m|\033[0m"
	@cc $(FLAGS) -o $(NAME) $(OBJS)
	@echo "\033[1;32mphilo ready ✓\033[0m"

$(OBJSDIR)%.o: $(SRCSDIR)%.c
	@cc $(FLAGS) -c $< -o $@

$(OBJSDIR):
	@mkdir -p $(OBJSDIR)
	@echo "\033[1;35mphilo compiling... |\033[0m\033[45m\c"

clean:
	@rm -rf $(OBJSDIR)
	@echo "\033[1;31mphilo objects deleted\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@echo "\033[1;31mphilo binary file deleted\033[0m"

re: fclean all

.PHONY: all clean fclean re

