/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asou <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 04:10:15 by asou              #+#    #+#             */
/*   Updated: 2024/12/28 10:33:07 by asou             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*extract_line(char	*bucket)
{
	size_t	i;
	char	*result;

	i = 0;
	while (bucket[i] && bucket[i] != '\n')
		i++;
	result = ft_substr(bucket, 0, i + (bucket[i] == '\n'));
	return (result);
}

static char	*update_bucket(char *bucket)
{
	size_t	i;
	size_t	bucket_len;
	char	*result;

	i = 0;
	bucket_len = ft_strlen(bucket);
	while (bucket[i] && bucket[i] != '\n')
		i++;
	result = ft_substr(bucket, i + (bucket[i] == '\n'), bucket_len - i);
	return (result);
}

static char	*gnl_internals(int fd, char *cup, char *bucket)
{
	ssize_t		bytes_read;
	char		*temp;

	while (!ft_strchr(bucket, '\n'))
	{
		bytes_read = read(fd, cup, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(bucket), bucket = NULL, NULL);
		if (bytes_read == 0)
			return (bucket);
		cup[bytes_read] = '\0';
		temp = ft_strjoin(bucket, cup);
		if (!temp)
			return (free(bucket), bucket = NULL, NULL);
		free(bucket);
		bucket = temp;
	}
	return (bucket);
}

char	*get_next_line(int fd)
{
	static char	*bucket;
	char		*temp;
	char		*line;
	char		*cup;

	if (BUFFER_SIZE < 0)
		return (NULL);
	cup = (char *)malloc(sizeof(char) * ((size_t)BUFFER_SIZE + 1));
	if (!cup)
		return (NULL);
	bucket = gnl_internals(fd, cup, bucket);
	free(cup);
	if (!bucket || !*bucket)
		return (free(bucket), bucket = NULL, NULL);
	line = extract_line(bucket);
	temp = update_bucket(bucket);
	if (!temp)
		return (free(bucket), free(line), bucket = NULL, NULL);
	free(bucket);
	bucket = temp;
	return (line);
}
