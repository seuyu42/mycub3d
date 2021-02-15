# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seuyu <seuyu@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/06/30 16:50:40 by seuyu             #+#    #+#              #
#    Updated: 2021/02/14 01:13:32 by seuyu            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


#임시 버전


NAME = cub3d

SRCS = re_main.c re_pars.c re_cub3d.c map_pars.c get_next_line.c get_next_line_utils.c init_malloc.c

CC = gcc

CFLAGS = -Wall -Wextra -Werror

MLXFLAGS = -framework Appkit -framework OpenGL

MLX = libmlx.dylib

MLXPATH = mlx

OBJS = $(SRCS:.c=.o)

OPTION = -c

all : $(NAME)

# $(NAME) : $(OBJS)
# 	cd $(LIBFT); make all
# 	cp $(LIBFT)/$(LIBFT_A) ./$(NAME)
# 	ar rcs $(NAME) $(OBJS)

$(NAME) : $(OBJS) $(MLX)
	$(CC) $(CFLAGS) -L$(MLXPATH) -lmlx $(MLXFLAGS) -o $(NAME) $(OBJS) $(MLX)


$(MLX):
	cd $(MLXPATH); make all
	cp ./mlx/$(MLX) ./

.c.o :
	$(CC) $(CFLAGS) $(OPTION) $< -o $@

clean :
	rm -f $(OBJS)
	cd $(MLXPATH); make clean

fclean : clean
	rm -f $(NAME)
	cd $(MLXPATH); make clean

re : fclean all

.PHONY: all clean fclean re
