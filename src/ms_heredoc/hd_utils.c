/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:45:29 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 03:56:23 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_itoa_buf(int n, char *buf, size_t size)
{
	int		len;
	char	tmp[12];
	int		i;

	len = 0;
	i = 0;
	if (n == 0)
		tmp[i++] = '0';
	else
	{
		if (n < 0)
		{
			if (size-- > 1)
				buf[len++] = '-';
			n = -n;
		}
		while (n > 0)
		{
			tmp[i++] = (n % 10) + '0';
			n /= 10;
		}
	}
	while (i-- > 0 && len < (int)size)
		buf[len++] = tmp[i];
	return (len);
}

int	handle_specifier(va_list args, char spec, char **dest, size_t *remaining)
{
	const char	*s;
	char		num_buf[12];
	int			num_len;
	int			i;

	if (spec == 's')
	{
		s = va_arg(args, const char *);
		if (!s)
			s = "(null)";
		i = 0;
		while (s[i] && *remaining > 1)
		{
			*(*dest)++ = s[i++];
			(*remaining)--;
		}
		return (ft_strlen(s));
	}
	else if (spec == 'd')
	{
		num_len = ft_itoa_buf(va_arg(args, int), num_buf, sizeof(num_buf));
		i = 0;
		while (i < num_len && *remaining > 1)
		{
			*(*dest)++ = num_buf[i++];
			(*remaining)--;
		}
		return (num_len);
	}
	return (0);
}

int	ft_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list	args;
	char	*dest;
	size_t	remaining;
	int		total;

	dest = str;
	remaining = size;
	total = 0;
	va_start(args, format);
	while (*format && remaining > 1)
	{
		if (*format == '%' && *(format + 1))
		{
			total += handle_specifier(args, *(format + 1), &dest, &remaining);
			format += 2;
		}
		else
		{
			if (remaining-- > 1)
				*dest++ = *format;
			total++;
			format++;
		}
	}
	if (size > 0)
		*dest = '\0';
	va_end (args);
	return (total);
}

char	*hd_remove_quotes(char *input)
{
	char	*return_var;
	int		i;

	return_var = NULL;
	i = 1;
	return_var = malloc(sizeof(char) * (ft_strlen(input) - 1));
	if (!return_var)
		return (NULL);
	while (input[i] && (unsigned long)i < (ft_strlen(input) - 1))
	{
		return_var[i - 1] = input[i];
		i++;
	}
	return_var[i - 1] = '\0';
	free (input);
	return (return_var);
}

void	eof_quote_remove(t_token *tokens)
{
	if (tokens->type == TOKEN_HEREDOC)
	{
		if (tokens->next->eof_envvar && tokens->next->eof_inquote)
			tokens->next->value = hd_remove_quotes(tokens->next->value);
	}
}
