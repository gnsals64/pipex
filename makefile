NAME = pipex
CC = cc
FLAG = -Wall -Werror -Wextra
SRCS = ./src/pipex.c
OBJS = ${SRCS :.c=.o}
LIBFT = ./libft

all : ${NAME}

${NAME} :
	@make -C ${LIBFT}
	${CC} -g ${FLAG} ${SRCS} ${LIBFT}/libft.a -o ${NAME}

clean :
	@make clean -C ${LIBFT}
	@rm -f ${OBJS}

fclean : clean
	@make fclean -C ${LIBFT}
	@rm -f ${NAME}

re:	fclean all
