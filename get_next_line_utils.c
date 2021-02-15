/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seuyu <seuyu@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 15:52:30 by seuyu             #+#    #+#             */
/*   Updated: 2021/02/04 01:58:01 by seuyu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strdup(const char *s1)
{
	size_t	len;
	size_t	i;
	char	*str;

	i = 0;
	if (!s1)
		return (0);
	len = ft_strlen((char *)s1);
	if (!(str = (char *)malloc(sizeof(char) * len + 1)))
		return (0);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	srclen;

	if (!src)
		return (0);
	i = 0;
	srclen = 0;
	if (size != 0)
	{
		while (src[i] != '\0' && i < size - 1)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	while (src[srclen] != '\0')
		srclen++;
	return (srclen);
}

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	dstlen;
	size_t	srclen;
	size_t	i;

	dstlen = 0;
	srclen = 0;
	i = 0;
	dstlen = ft_strlen(dest);
	srclen = ft_strlen(src);
	if (size <= dstlen)
		return (srclen + size);
	while (src[i] != '\0' && i + dstlen + 1 < size)
	{
		dest[i + dstlen] = src[i];
		i++;
	}
	dest[i + dstlen] = '\0';
	return (srclen + dstlen);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1len;
	size_t	s2len;
	char	*str;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1 || !s2)
		return (!s1 ? ft_strdup(s2) : ft_strdup(s1));
	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	if (!(str = (char *)malloc(sizeof(char) * (s1len + s2len + 1))))
		return (NULL);
	ft_strlcpy(str, s1, s1len + 1);
	ft_strlcat(str + s1len, s2, s2len + 1);
	return (str);
}



int	ft_isspace(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

int ft_isdigit(char c)
{
	if (('0' <= c && c <= '9'))
			return (1);
	return (0);
}

int			ft_atoi(const char *str)
{
	int i;
	int minus;
	int num;

	i = 0;
	minus = 1;
	num = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			minus *= -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (num * minus);
}


static char		*ft_strndup(char const *src, int size)
{
	int		i;
	char	*dst;

	i = 0;
	if (!(dst = (char*)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	while (i < size)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

static int		ft_str_count(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i] != c && s[i])
			count++;
		while (s[i] != c && s[i])
			i++;
	}
	return (count);
}

static void		ft_freeall(char **split)
{
	size_t i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static int		ft_char_count(char **split, char const *s, char c, int num)
{
	int	j;
	int	count;

	j = 0;
	count = 0;
	while (num > 0)
	{
		j = 0;
		while (*s == c && *s)
			s++;
		if (*s != c && *s)
			count++;
		while (*s != c && *s)
		{
			s++;
			j++;
		}
		if (!(split[count - 1] = ft_strndup(s - j, j)))
		{
			ft_freeall(split);
			return (0);
		}
		num--;
	}
	return (1);
}

char			**ft_split(char const *s, char c)
{
	int		strnum;
	char	**split;

	if (!s)
		return (NULL);
	strnum = ft_str_count(s, c);
	if (!(split = (char**)malloc(sizeof(char*) * (strnum + 1))))
		return (NULL);
	split[strnum] = NULL;
	if (!ft_char_count(split, s, c, strnum))
		return (NULL);
	return (split);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s && *s != c)
		s++;
	if (*s == c)
		return ((char *)s);
	else
		return (NULL);
}
