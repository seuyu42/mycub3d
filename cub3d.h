#ifndef CUB3D_H
# define CUB3D_H

# define	WIN_MAX_X			2560
# define	WIN_MAX_Y			1440
# define BUFFER_SIZE 32


//파싱맵을 쓰면 필요 x
// # define MAP_W 24
// # define MAP_H 24

//그대로
# define T_W 64
# define T_H 64

# define K_ESC 53
# define KEY_EVENT_PRESS 2
# define KEY_EVENT_RELEASE 3
# define KEY_EVENT_EXIT 17

#define X_EVENT_KEY_PRESS	2
# define X_EVENT_KEY_RELEASE	3
#define X_EVENT_KEY_EXIT	17

# define KEY_UP 126
# define KEY_DOWN 125
# define KEY_LEFT 124
# define KEY_RIGHT 123
# define W_W 600


//나중엔 스프라이트 파싱해서 개수 파악해야함.

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>

#include <stdio.h>
#include <math.h>
#include "mlx/mlx.h"
#include "get_next_line.h"


typedef struct s_sprite
{
	double x;
	double y;
	// int		texture;
}			t_sprite;

typedef struct s_pars
{
	int win_wid;
	int win_height;

	char *NO_path;
	char *SO_path;
	char *WE_path;
	char *EA_path;
	char *SP_path;

	int ceil_clr;
	int ceil_R;
	int ceil_G;
	int ceil_B;

	int floor_clr;
	int floor_R;
	int floor_G;
	int floor_B;

	int **map;
	int map_idx;

	double posX;
	double posY;

	double dirX;
	double dirY;
	double planeX;
	double planeY;

	int sp_num;
	t_sprite *sprite;

	int scr_x;
	int scr_y;
} 		t_pars;

typedef struct s_img
{
	int bpp;
	int size_l;
	int endian;
	int img_W;
	int img_H;

	void *ptr;
	int *data;
}	t_img;

typedef struct s_info
{
	int		**map;
	int		**buf;
	int		**texture;
	void	*mlx;
	void	*win;
	t_img	img;

	//t_calc
	double dirX;
	double dirY;
	double planeX;
	double planeY;
	double moveSpeed;
	double rotSpeed;

	//t_key
	int key_up;
	int key_down;
	int key_left;
	int key_right;
	int key_esc;
	double z_buffer[W_W];

	t_pars *pars;


}			t_info;

typedef struct s_pair
{
	double first;
	int	second;
}		t_pair;

int			get_next_line(int fd, char **line);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strdup(char const *s1);
size_t		ft_strlcat(char *dest, const char *src, size_t size);
size_t		ft_strlcpy(char *dest, const char *src, size_t size);
// gnl

size_t		ft_strlen(const char *str);
int			ft_isspace(char c);
int			ft_isdigit(char c);
int			ft_atoi(const char *str);
char		**ft_split(char const *s, char c);
char		*ft_strchr(const char *s, int c);
// utils


void init_elem(t_pars *pars);
void cub3d_pars(int fd, t_pars *pars);
int check_map(char *line);
int map_line_check(char *line);
int *char_to_int_line(char *line);
void count_sp(t_pars *pars);
int valid_map(t_pars *pars);
int check_elem(t_pars *pars);
int pars_map(int *line, t_pars *pars);



void	key_update(t_info *info);
int		key_press(int key, t_info *info);
int		key_release(int key, t_info *info);
int		main_loop(t_info *info);
void	load_tex(t_info *info);
void 	info_init(t_info *info);
void 	buff_init(t_info *info);
void 	tex_init(t_info *info);
// void 	zbuffer_mal(t_info *info);




#endif
