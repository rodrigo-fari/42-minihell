/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps_expander_split_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 00:16:37 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 03:58:33 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expander_wc(char *str, char c)
{
	int	i;
	int	e_wc;

	i = 0;
	e_wc = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == '\'' || str[i] == '\"' || str[i] == ' '))
		{
			e_wc++;
			i++;
		}
		while (str[i] && str[i] == c)
			i++;
		if (str[i])
			e_wc++;
		while (str[i] && ft_isalnum(str[i]))
			i++;
	}
	return (e_wc);
}

char	**expander_splitter(char **ret_split, char *str, char delim)
{
	int	i;
	int	j;
	int	cursor;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (is_expander_char(str[i]))
			handle_special_char(&i, &j, ret_split, str);
		else if (str[i] == delim)
			i++;
		else
		{
			cursor = i;
			while (is_valid_char(str[cursor], delim))
				cursor++;
			ret_split[j++] = ft_substr(str, i, cursor - i);
			i = cursor;
		}
	}
	return (ret_split);
}

char	**expander_split(char *str, char delim)
{
	char	**ret_split;

	ret_split = ft_calloc(sizeof(char *), (expander_wc(str, delim) + 1));
	if (!ret_split)
		return (NULL);
	return (expander_splitter(ret_split, str, delim));
}

int	is_expander_char(char c)
{
	return (c == '\'' || c == '\"' || c == ' ');
}

void	handle_special_char(int *i, int *j, char **ret_split, char *str)
{
	ret_split[(*j)] = ft_substr(str, *i, 1);
	(*j)++;
	(*i)++;
}
