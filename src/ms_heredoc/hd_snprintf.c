/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 00:07:35 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/29 00:52:49 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_char(char c, t_snprintf_data *data)
{
	if (data->remaining > 1)
	{
		*data->dest++ = c;
		data->remaining--;
	}
	data->total++;
}

int	handle_string(t_snprintf_data *data)
{
	char	*str;
	int		written;

	str = va_arg(*data->args, char *);
	if (!str)
		str = "(null)";
	written = 0;
	while (*str && data->remaining > 1)
	{
		write_char(*str++, data);
		written++;
	}
	return (written);
}

int	handle_number(t_snprintf_data *data)
{
	long	num;
	char	buffer[12];
	int		len;
	int		written;

	num = va_arg(*data->args, int);
	written = 0;
	if (num < 0)
	{
		write_char('-', data);
		num = -num;
		written++;
	}
	len = 0;
	while (num > 0 || len == 0)
	{
		buffer[len++] = (num % 10) + '0';
		num /= 10;
	}
	while (len-- && data->remaining > 1)
	{
		write_char(buffer[len], data);
		written++;
	}
	return (written);
}

void	handle_specifier(char spec, t_snprintf_data *data)
{
	if (spec == 's')
		handle_string(data);
	else if (spec == 'd')
		handle_number(data);
}

void	process_format(const char **format, t_snprintf_data *data)
{
	while (**format && data->remaining > 1)
	{
		if (**format == '%' && *(*format + 1))
		{
			handle_specifier(*(*format + 1), data);
			*format += 2;
		}
		else
		{
			write_char(*(*format)++, data);
		}
	}
}
