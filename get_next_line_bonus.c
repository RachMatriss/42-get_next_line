/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramedjra <ramedjra@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 12:37:11 by ramedjra          #+#    #+#             */
/*   Updated: 2023/01/29 13:22:27 by ramedjra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*FT_CHECK used when spot found or eof
** takes the static buffer, the in_file pointer, the current line 
** and the spot for occurence \n
** if eof search for \0, store address in spot
** create line with static buffer, from 0 to end. 
** return line
** ft_check: buffer:				remaining chars for new line
** ft_check: in_file:				0 = EOF, 1 = in file
** ft_check: line:					current line
** ft_check: spot:					occurence of \n or \0
** ft_check: tmp:					temp. ptr for new remainder
** ---
*/
char	*ft_check(char **buffer, int *in_file, char *line, char *spot)
{
	char	*tmp;

	if (*buffer && *in_file == 0)
		spot = ft_strchr(*buffer, '\0');
	line = ft_substr(*buffer, 0, spot - *buffer + *in_file);
	if (!line)
		return (NULL);
	tmp = ft_strjoin(spot + *in_file, "");
	if (!tmp)
		return (NULL);
	free(*buffer);
	*buffer = tmp;
	return (line);
}

/* FT_LINE read line function 
*  used when spot not found and still in file
* takes fd and *in_file, reads from file to buffer
* returns buffer.
** ft_line: fd:				filedescriptor 
** ft_line: in_file: 			0 = EOF, 1 = in file
** ft_line: buffer: 			buffer for read
** ft_line: byte_read:		        return val for read
** ---
*/
char	*ft_line(int fd, int *in_file)
{
	char	*buffer;
	int		byte_read;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	byte_read = read(fd, buffer, BUFFER_SIZE);
	if (byte_read == 0)
		*in_file = 0;
	else if (byte_read == -1)
	{
		free(buffer);
		return (NULL);
	}
	buffer[byte_read] = '\0';
	return (buffer);
}

/* buffer_check( buffer, fd, in_file and line)
** if buffer is existing, check for \n and store address in spot
** if no spot found, read next buffer and add to remainder, 
** else: spot found or eof.
** return line
** buffer_check: buffer: 	remaining chars for new line
** buffer_check: fd:			filedescriptor 
** buffer_check: in_file:		0 = EOF, 1 = in file
** buffer_check: line:		current line
** buffer_check: spot:		occurence of \n
** buffer_check: tmp:		ptr to new read buffer
** buffer_check: tmpX:		ptr to concatenated str of remainder + temp
** ---
*/
char	*buffer_check(char **buffer, int fd, int *in_file, char *line)
{
	char	*spot;
	char	*tmp;
	char	*tmpx;

	spot = NULL;
	if (*buffer)
		spot = ft_strchr(*buffer, '\n');
	if (!spot && *in_file == 1)
	{
		tmp = ft_line(fd, in_file);
		if (!tmp)
			return (NULL);
		tmpx = ft_strjoin(*buffer, tmp);
		if (!tmpx)
			return (NULL);
		free(*buffer);
		free(tmp);
		*buffer = tmpx;
		line = buffer_check(buffer, fd, in_file, line);
	}
	else
		line = ft_check(buffer, in_file, line, spot);
	return (line);
}

/* GET_NEXT_LINE
** takes fd and error-checks it
** if no static buffer found read new buffer into it
** set line to the return of buffer_check
**if no new line found, return NULL
** return line
** get_next_line: fd:				filedescriptor 
** get_next_line: buffer:	                static buffer chars for new line
** get_next_line: line:				current line
** get_next_line: in_file:			0 = EOF, 1 = in file
** ---
*/
char	*get_next_line(int fd)
{
	static char	*buffer[1024];
	char		*line;
	int			in_file;

	in_file = 1;
	if (fd == -1 || BUFFER_SIZE <= 0)
		return (NULL);
	if (buffer[fd] == NULL)
		buffer[fd] = ft_line(fd, &in_file);
	line = buffer_check(&buffer[fd], fd, &in_file, NULL);
	if (!line)
	{
		free(buffer[fd]);
		buffer[fd] = NULL;
	}
	return (line);
}
