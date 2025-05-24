/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ev_print_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:18:36 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/24 15:45:42 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_env *env)
{
	t_env	*temp;

	env = get_env(NULL);
	temp = env;
	while (temp)
	{
		if (temp->has_equal)
		{
			printf("%s", temp->key);
			printf("=");
			if (temp->value)
				printf("%s", temp->value);
			printf("\n");
		}
		temp = temp->next;
	}
}
