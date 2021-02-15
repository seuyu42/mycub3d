#include "cub3d.h"

void init_elem(t_pars *pars)
{
	pars->win_wid = -1;
	pars->win_height = -1;
	pars->map_idx = 0;
	pars->sp_num = 0;
	pars->NO_path = NULL;
	pars->SO_path = NULL;
	pars->WE_path = NULL;
	pars->EA_path = NULL;
	pars->SP_path = NULL;
	pars->ceil_R = -1;
	pars->ceil_G = -1;
	pars->ceil_B = -1;
	pars->floor_R = -1;
	pars->floor_G = -1;
	pars->floor_B = -1;
}

int check_elem(t_pars *pars)
{
	if(pars->win_wid == -1 || pars->win_height == -1)
		return (0);
	if(pars->NO_path == NULL || pars->SO_path == NULL
	|| pars->WE_path == NULL || pars->EA_path == NULL
	|| pars->SP_path == NULL)
		return (0);
	if(pars->ceil_R == -1 || pars->ceil_G == -1 || pars->ceil_B == -1)
		return (0);
	if(pars->floor_R == -1 || pars->floor_G == -1 || pars->floor_B == -1)
		return (0);
	return(1);
}
// id를 체크해서 각각 리턴값을 반환하는 함수.
int 	check_id(char *line)
{
	if (line[0] == 'R')
		return (1);
	else if (line[0] == 'N' && line[1] == 'O')
		return (2);
	else if (line[0] == 'S' && line[1] == 'O')
		return (3);
	else if (line[0] == 'W' && line[1] == 'E')
		return (4);
	else if (line[0] == 'E' && line[1] == 'A')
		return (5);
	else if (line[0] == 'S' && line[1] != '0')
		return (6);
	else if (line[0] == 'C')
		return (7);
	else if (line[0] == 'F')
		return (8);
	else if (line[0] == '\0')
		return (9);
	return (0);
}


// 숫자로 된 문자열인지 확인
int is_numstr(char *str)
{
	int i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
// 이중배열을 메모리 해제해주는 함수.
void free_split(char **split)
{
	int i = 0;
	while(split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
void free_int_split(int **split)
{
	int i = 0;
	while(split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
// id = R에 해당하는 창크기를 가져오는 함수.
// TODO : 최대 크기 처리해주어야한다.
int pars_win(char *line, t_pars *pars)
{
	char **split;

	split = ft_split(line, ' ');
	int i = 0;
	while (split[i])
		i++;
	if (i != 3 || !(is_numstr(split[1])) || !(is_numstr(split[2])))
		return (0);
	if (pars->scr_x < (pars->win_wid = ft_atoi(split[1])))
		pars->win_wid = pars->scr_x;
	if (pars->scr_y < (pars->win_height = ft_atoi(split[2])))
		pars->win_height = pars->scr_y;
	free_split(split);
	return (1);
}

int pars_tex(int id, char *line, t_pars *pars)
{
	char **split;
	char *path;
	int i = 0;

	split = ft_split(line, ' ');
	while (split[i])
		i++;
	if (i != 2 || split[1][0] != '.' || split[1][1] != '/')
		return (0);
	path = ft_strdup(split[1] + 2);
	//만약 split[1]을 옮겨버리면 free할때 에러가 생김.
	if (id == 2)
		pars->NO_path = path;
	else if(id == 3)
		pars->SO_path = path;
	else if(id == 4)
		pars->WE_path = path;
	else if(id == 5)
		pars->EA_path = path;
	else if(id == 6)
		pars->SP_path = path;
	free_split(split);
	return (1);
}

//16진수 변형 그냥안하고 꼼수 씀
void hex_color(t_pars *pars)
{
	pars->ceil_clr = (pars->ceil_R * 16 * 16 * 16 * 16) + (pars->ceil_G * 16 * 16) + pars->ceil_B;
	pars->floor_clr = (pars->floor_R * 16 * 16 * 16 * 16) + (pars->floor_G * 16 * 16) + pars->floor_B;
}
//color파싱한거 값으로 적용하기 16진수 변경
int pars_color(int id, char *line, t_pars *pars)
{
	line++;
	while (ft_isspace(*line))
		line++;
	int i = 0;
	char **split;
	split = ft_split(line, ',');
	while (split[i])
		i++;
	if (i != 3 || !is_numstr(split[0]) || !is_numstr(split[1]) || !is_numstr(split[2]))
		return(0);
	if(id == 7)   // 비트마스크 활용법 참고.
	{
		pars->ceil_R = ft_atoi(split[0]);   //배열로 처리해서 깔금하게  할 지 결정
		pars->ceil_G = ft_atoi(split[1]);
		pars->ceil_B = ft_atoi(split[2]);
	}
	else if(id == 8)
	{
		pars->floor_R = ft_atoi(split[0]);
		pars->floor_G = ft_atoi(split[1]);
		pars->floor_B = ft_atoi(split[2]);
	}
	hex_color(pars);
	free_split(split);
	return(1);
}

void cub3d_pars(int fd, t_pars *pars)
{
	char *line;

	init_elem(pars);

	while (get_next_line(fd, &line) > 0)
	{
		if (check_id(line) == 1)
			pars_win(line, pars);
		else if (check_id(line) > 1 && check_id(line) < 7)
			pars_tex(check_id(line), line, pars);
		else if (check_id(line) == 7 || check_id(line) == 8)
			pars_color(check_id(line), line, pars);
		else if(map_line_check(line) && check_map(line))
		{
			int *tmp;
			tmp = char_to_int_line(line);
			pars_map(tmp, pars);
		}
		free(line);
	}
	count_sp(pars);
	if (!(pars->sprite = (t_sprite *)malloc(sizeof(t_sprite) * (pars->sp_num + 1))))
		return ;
	valid_map(pars);
}
