/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seuyu <seuyu@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/29 14:35:14 by seuyu             #+#    #+#             */
/*   Updated: 2021/02/04 01:58:37 by seuyu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define BUFFER_SIZE 32
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>

int			get_next_line(int fd, char **line);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strdup(char const *s1);
size_t		ft_strlcat(char *dest, const char *src, size_t size);
size_t		ft_strlcpy(char *dest, const char *src, size_t size);

size_t		ft_strlen(const char *str);
int			ft_isspace(char c);
int			ft_isdigit(char c);
int			ft_atoi(const char *str);
char		**ft_split(char const *s, char c);
char		*ft_strchr(const char *s, int c);

#endif
