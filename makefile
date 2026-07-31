# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ibouzekr <ibouzekr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/12 21:13:39 by ibouzekr          #+#    #+#              #
#    Updated: 2026/07/13 00:56:29 by ibouzekr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGES= -Wall -Wextra -Werror -pthread 

SRCS = 	main.c parsing.c init_data.c coder_routine.c request_sender.c \
		min_heap.c simulation_check.c dongle_check.c monitor.c dongles.c \
		free_ressources.c utils.c threads.c init_coders.c init_dongles.c release_dongles.c

OBJS = $(SRCS:.c=.o)

NAME = codexion

all: $(NAME)

$(NAME): $(OBJS) 
	cc $(CFLAGES) $(OBJS) -o codexion

%.o: %.c codexion.h
	cc $(CFLAGES) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
