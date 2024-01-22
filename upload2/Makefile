GREENGREEN = \033[38;5;46m
RED = \033[0;31m
GREY = \033[38;5;240m
RESET = \033[0m

NAME =     fractol

CC =         gcc
CFLAGS =     -Wall -Wextra -Werror -O3
RM =         rm -rf

DIR_H = headers/
DIR_S =	srcs/
CREATE_DIR_O = @mkdir -p objs
DIR_O =	objs/

SRCS_LIST =	init.c mandelbrot.c main.c hook.c julia.c colors.c burningship.c

SRCS =		${addprefix ${DIR_S}, ${SRCS_LIST}}

OBJS =		${SRCS:${DIR_S}%.c=${DIR_O}%.o}

# Compile la Libft
DIR_LIBFT = libft/
LIBFT_INC = -I ${DIR_LIBFT}
LIBFT =	${DIR_LIBFT}libft.a
FT_LNK = -L ${DIR_LIBFT} -l ft

# Compile la MiniLibX suivant l'OS
ifeq (${shell uname}, Linux)
	DIR_MLX = mlx_linux/
	MLX_LNK	= -L $(DIR_MLX) -lmlx -lXext -lX11 -lbsd -lm
else
	DIR_MLX = mlx_macos/
	MLX_LNK	= -L $(DIR_MLX) -l mlx -framework OpenGL -framework AppKit
endif
MLX_INC = -I ${DIR_MLX}
MLX =	${DIR_MLX}libmlx.a

LIBS = ${FT_LNK} ${MLX_LNK}

# ${NAME}: title ${LIBFT} ${MLX} ${OBJS}
${NAME}: ${LIBFT} ${MLX} ${OBJS}
	@echo "$(RESET)[$(GREENGREEN)${NAME}$(RESET)]: Fractol Objects were created${GREY}"
	${CC} ${OBJS} ${LIBS} -o ${NAME}
	@echo "$(RESET)[$(GREENGREEN)${NAME}$(RESET)]: Fractol created !"

title:
	@echo "$(GREENGREEN) ██████╗██████╗  █████╗  ██████╗████████╗    ██████╗ ██╗$(RESET)"
	@echo "$(GREENGREEN) ██╔════╝██╔══██╗██╔══██╗██╔════╝╚══██╔══╝   ██╔═══██╗██║$(RESET)"
	@echo "$(GREENGREEN) █████╗  ██████╔╝███████║██║        ██║█████╗██║   ██║██║$(RESET)"
	@echo "$(GREENGREEN) ██╔══╝  ██╔══██╗██╔══██║██║        ██║╚════╝██║   ██║██║$(RESET)"
	@echo "$(GREENGREEN) ██║     ██║  ██║██║  ██║╚██████╗   ██║      ╚██████╔╝███████╗$(RESET)"
	@echo "$(GREENGREEN) ╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝   ╚═╝       ╚═════╝ ╚══════╝$(RESET)"

${LIBFT}:
	@echo "[$(GREENGREEN)${NAME}$(RESET)]: Creating Libft...${GREY}"
	${MAKE} -sC ${@D}
	@echo "$(RESET)[$(GREENGREEN)${NAME}$(RESET)]: Libft Objects were created"

${MLX}:
	@echo "[$(GREENGREEN)${NAME}$(RESET)]: Creating MLX...$(GREY)"
	${MAKE} -sC ${@D}
	@echo "$(RESET)[$(GREENGREEN)${NAME}$(RESET)]: MLX Objects were created"
	@echo "${RESET}[$(GREENGREEN)${NAME}$(RESET)]: Creating Fractol Objects...${GREY}"

all: ${NAME}

${DIR_O}%.o:${DIR_S}%.c
	@printf "\033[38;5;240m"
	@mkdir -p ${DIR_O}
	${CC} ${CFLAGS} ${LIBFT_INC} ${MLX_INC} -I ${DIR_H} -o $@ -c $<

clean:
	@echo "$(RED) ██████╗██╗     ███████╗ █████╗ ███╗   ██╗██╗███╗   ██╗ ██████╗$(RESET)"
	@echo "$(RED) ██╔════╝██║     ██╔════╝██╔══██╗████╗  ██║██║████╗  ██║██╔════╝$(RESET)"
	@echo "$(RED) ██║     ██║     █████╗  ███████║██╔██╗ ██║██║██╔██╗ ██║██║  ███╗$(RESET)"
	@echo "$(RED) ██║     ██║     ██╔══╝  ██╔══██║██║╚██╗██║██║██║╚██╗██║██║   ██║$(RESET)"
	@echo "$(RED) ╚██████╗███████╗███████╗██║  ██║██║ ╚████║██║██║ ╚████║╚██████╔╝$(RESET)"
	@echo "$(RED)  ╚═════╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝  ╚═══╝ ╚═════╝ $(RESET)"
	@echo "[$(RED)${NAME}$(RESET)]: Cleaning Fractol Objects...${GREY}"
	${RM} ${OBJS}
	${RM} ${DIR_O}
	@echo "[$(RED)${NAME}$(RESET)]: Fractol Objects were cleaned${GREY}"

libclean:
	@echo "${RESET}[$(RED)${NAME}$(RESET)]: Cleaning MLX...${GREY}"
	${MAKE} -sC ${DIR_MLX} clean
	@echo "${RESET}[$(RED)${NAME}$(RESET)]: MLX Objects were cleaned"
	@echo "${RESET}[$(RED)${NAME}$(RESET)]: Cleaning Libft...${GREY}"
	${MAKE} -sC ${DIR_LIBFT} fclean
	@echo "${RESET}[$(RED)${NAME}$(RESET)]: Libft Objects were cleaned"

fclean: clean libclean
	@echo "${RESET}[$(RED)${NAME}$(RESET)]: Cleaning Fractol...${GREY}"
	${RM} ${NAME}
	@echo "${RESET}[$(RED)${NAME}$(RESET)]: Fractol Executable was cleaned"

re: fclean all

.PHONY: all clean fclean re title libclean 
-----------------------------10158718905304871933744908491--
