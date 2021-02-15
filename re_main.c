#include "cub3d.h"


int main()
{

	int fd = open("map_example.cub" , O_RDONLY);
	t_info info;


	info.mlx = mlx_init();
	mlx_get_screen_size(info.mlx, &info.pars->scr_x, &info.pars->scr_y);

	cub3d_pars(fd, info.pars);

	info_init(&info);
	buff_init(&info);
	tex_init(&info);

	// zbuffer_mal(&info);

	load_tex(&info);
	info.win = mlx_new_window(info.mlx, info.pars->win_wid, info.pars->win_height, "seuyu");
	info.img.ptr = mlx_new_image(info.mlx, info.pars->win_wid, info.pars->win_height);
	info.img.data = (int *)mlx_get_data_addr(info.img.ptr, &info.img.bpp, &info.img.size_l, &info.img.endian);
	mlx_loop_hook(info.mlx, &main_loop, &info);
	mlx_hook(info.win, KEY_EVENT_PRESS, 0, &key_press, &info);
	mlx_hook(info.win, KEY_EVENT_RELEASE, 0, &key_release, &info);
	mlx_loop(info.mlx);
}



