/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps_pipes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 16:45:28 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/29 02:19:10 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_pipes(char **commands)
{
	int	i;

	i = 0;
	while (commands[i])
	{
		if (commands[i][0] == '|' && i == 0)
		{
			print_error("Minishell: syntax error near unexpected token: |");
			return (false);
		}
		if (quote_verifier(commands[i]) && commands[i + 1])
			i++;
		if (commands[i][0] == '|')
		{
			if (!commands[i + 1] || !commands[i - 1])
			{
				print_error("Minishell: syntax error near unexpected token: |");
				return (false);
			}
		}
		i++;
	}
	return (true);
}
