/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exec_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 18:04:09 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/30 18:04:31 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**prepare_exec_commands(char **commands)
{
	char	**new_commands;
	int		i;
	int		j;
	int		valid_count;

	valid_count = 0;
	j = 0;
	i = 0;
	while (commands[i])
	{
		if (ft_strcmp(commands[i], "") != 0)
			valid_count++;
		i++;
	}
	if (valid_count == 0)
		return (NULL);
	new_commands = ft_calloc(sizeof(char *), (valid_count + 1));
	i = 0;
	while (commands[i])
	{
		if (ft_strcmp(commands[i], "") != 0)
			new_commands[j++] = ft_strdup(commands[i]);
		i++;
	}
	return (new_commands);
}
