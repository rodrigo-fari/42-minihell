/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_snprintf_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 00:37:10 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/29 00:37:53 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_snprintf(char *str, size_t size, const char *format, ...)
{
	t_snprintf_data	data;
	va_list			args;

	if (!str || !format)
		return (-1);
	data.dest = str;
	data.remaining = size;
	data.total = 0;
	va_start(args, format);
	data.args = &args;
	process_format(&format, &data);
	if (size > 0)
		*data.dest = '\0';
	va_end(args);
	return (data.total);
}
