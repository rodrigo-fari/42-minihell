/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:45:29 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/29 00:51:02 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
