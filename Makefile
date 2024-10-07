# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gigardin <gigardin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/27 23:20:01 by gigardin          #+#    #+#              #
#    Updated: 2024/09/27 23:20:01 by gigardin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

#--------------FLAGS----------------------------#
CFLAGS = -Wall -Werror -Wextra -g3 -Iincludes
LDFLAGS = -lpthread
#--------------PATHS----------------------------#

OBJFOLDER = objs/
SOURCES = sources/

#--------------FILES----------------------------#
SRC = sources/action.c\
sources/create_threads.c\
sources/death.c\
sources/main.c\
sources/utils.c\
sources/init_mutex.c\

#--------------RULES----------------------------#

OBJFILES = $(subst $(SOURCES),$(OBJFOLDER),$(SRC:.c=.o))

all: $(OBJFOLDER) $(NAME)

v: all
	valgrind --tool=helgrind -s ./$(NAME)

$(OBJFOLDER):
	@mkdir -p $(OBJFOLDER)

$(NAME): $(OBJFILES)
	$(CC) $(OBJFILES) $(CFLAGS) $(LDFLAGS) -o $(NAME)

$(OBJFOLDER)%.o : $(SOURCES)%.c
	$(CC) $(CFLAGS) -c $< -o $@

#----------------CLEAN--------------------------#

clean:
	rm -f $(OBJFILES)
	rm -rf $(OBJFOLDER)

fclean: clean
	rm -f $(NAME)

gdb: all
	gdb --tui -ex 'set follow-fork-mode child' -ex 'b main' -ex 'b create_threads' -ex 'run' ./$(NAME)

re: fclean all

.PHONY: all clean fclean re gdb
