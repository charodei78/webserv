# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hleilani <hleilani@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/28 13:03:11 by hleilani          #+#    #+#              #
#    Updated: 2020/11/29 15:32:43 by hleilani         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

LIBC = libft/libft.a

HTTPSRC = $(shell find ./HTTP -name "*.cpp")

HELPERSRC = $(shell find ./Helpers -name "*.cpp")

EXCEPTIONSRC = $(shell find ./exceptions -name "*.cpp")

SERVERSRC = $(shell find ./Server -name "*.cpp")

SRC = ${HTTPSRC} ${HELPERSRC} ${EXCEPTIONSRC} ${SERVERSRC} ./main.cpp

OBJ = ${SRC:.cpp=.o}

CC = clang++

CFLAGS = -Wall -Wextra -Werror -std=c++98

${NAME}: ${OBJ}
	${CC} ${CFLAGS} ${OBJ} -o ${NAME}

all: ${NAME}


.cpp.o:
	${CC} ${CFLAGS} $< -c -o $@

bonus: all

clean:
	rm -f ${OBJ}

fclean:	clean
	rm -f ${NAME}

re: fclean all