# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/30 09:21:39 by lebarbos          #+#    #+#              #
#    Updated: 2023/12/03 17:08:55 by lebarbos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#CORES
RESET	= \033[0m
BLACK 	= \033[1;30m
RED 	= \033[1;31m
GREEN 	= \033[1;32m
YELLOW 	= \033[1;33m
BLUE	= \033[1;34m
MAGENTA	= \033[1;35m
CYAN 	= \033[1;36m
WHITE 	= \033[1;37m

#COMMANDS
CC	= cc
RM	= rm -rf

#FLAGS
CFLAGS 	= -Werror -Wextra -Wall -g #-fsanitize=address
LFLAGS	= -L ./libft -lft

#PATH
INC			= include
SRCS		= srcs
SRCS_BONUS	= bonus
LIBFT_PATH	= libft
OBJ			= objs
OBJ_BONUS	= objs_bonus

#FILES
NAME		= pipex
SRC_FILES	= pipex.c pipex_utils.c
OBJ_FILES	= $(SRC_FILES:%.c=%.o)
TARGET		= $(addprefix $(OBJ)/, $(OBJ_FILES))
#BONUS FILES
NAME_BONUS		= pipex_bonus
BONUS_FILES		= pipex_bonus.c
BONUS_OBJS		= $(BONUS_FILES:%.c=%.o)
BONUS_TARGET	= $(addprefix $(OBJ_BONUS)/, $(BONUS_OBJS))

#RULES
all: $(NAME)

$(NAME): $(OBJ) $(TARGET)
	make -C $(LIBFT_PATH)
	$(CC) $(CFLAGS) $(TARGET) $(LFLAGS) -o $(NAME)

$(OBJ)/%.o : $(SRCS)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC)

$(OBJ):
	mkdir -p $(OBJ)

clean:
	$(RM) $(OBJ)
	make clean -C $(LIBFT_PATH)
	

fclean: clean
	$(RM) $(OBJS) $(NAME)
	make fclean -C $(LIBFT_PATH)