/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_expander_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 03:55:03 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 03:57:56 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hd_atributes(t_token *current)
{
	if (ft_strchr(current->value, '\"')
		|| ft_strchr(current->value, '\''))
		current->eof_inquote = true;
	if (ft_strchr(current->value, '$'))
		current->eof_envvar = true;
}

char	*expand_vars(char *input)
{
	int		i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$')
			result = append_var(result, input, &i);
		else
		{
			result = append_char(result, input[i]);
			i++;
		}
	}
	return (result);
}
