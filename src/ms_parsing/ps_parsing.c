/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:43:40 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 17:02:13 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	pasring_verify(char **cmds, char *input)
{
	if (!ps_parsing(cmds))
	{
		ms_free(NULL, input, cmds, NULL);
		return (false);
	}
	return (true);
}

bool	ps_parsing(char **commands)
{
	int	i;

	i = 0;
	while (commands[i])
	{
		if (!parse_quotes(commands[i], 0, 0))
			return (false);
		i++;
	}
	if (!parse_redin(commands))
		return (false);
	if (!parse_redout(commands))
		return (false);
	if (!parse_pipes(commands))
		return (false);
	return (true);
}

bool	quote_verifier(char *input)
{
	if (input[0] && (input[0] == '\"' || input[0] == '\"'))
		return (true);
	else
		return (false);
}
