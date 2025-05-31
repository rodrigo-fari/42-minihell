/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps_quotes_and_expand_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:50:29 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/31 13:45:38 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *input)
{
	char	*ret_str;
	char	*tmp;
	int		i;

	i = 0;
	ret_str = NULL;
	while (input[i])
	{
		while (input[i] && (input[i] == '\"' || input[i] == '\''))
			i++;
		while (input[i] && input[i] != '\"' && input[i] != '\'')
		{
			tmp = append_char_to_string(ret_str, input[i]);
			if (ret_str)
				free(ret_str);
			ret_str = tmp;
			i++;
		}
		i++;
	}
	free(tmp);
	return (ret_str);
}
