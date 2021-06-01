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

SRCOBJ = ${SRC:.cpp=.o}

OBJ = ${notdir ${SRCOBJ}}

CC = clang++

CFLAGS = -Wall -Wextra -Werror

${NAME}: compile moveobjs

compile: ${SRCOBJ} ${OBJ}
	${CC} ${CFLAGS} ${OBJ} -o ${NAME}

moveobjs: ${OBJ}
	mkdir -p obj
	mv ${OBJ} obj

all: ${NAME}

.cpp.o:
	${CC} ${CFLAGS} $< -c

bonus: all

clean:
	rm -r obj

fclean:	clean
		rm -f ${NAME}

re: fclean all