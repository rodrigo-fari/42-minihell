/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:18:50 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 17:35:14 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_echo(t_token *tmp)
{
	bool				flag;
	struct sigaction	sa_old;
	struct sigaction	sa_ignore;

	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGPIPE, &sa_ignore, &sa_old);
	flag = true;
	if (!tmp->value[0])
	{
		write(STDOUT_FILENO, "\n", 1);
		sigaction(SIGPIPE, &sa_old, NULL);
		return ;
	}
	while (tmp)
	{
		tmp = tmp->next;
		while (tmp && flag_verify(tmp->value))
		{
			flag = false;
			if (!tmp->next)
			{
				sigaction(SIGPIPE, &sa_old, NULL);
				return ;
			}
			else
				tmp = tmp->next;
		}
		while (tmp)
		{
			ms_print_fd(tmp->value, 1);
			if (tmp->next)
				ms_print_fd(" ", 1);
			tmp = tmp->next;
		}
	}
	if (tmp == NULL && flag)
		ms_print_fd("\n", 1);
	sigaction(SIGPIPE, &sa_old, NULL);
}

bool	flag_verify(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '-')
		{
			i++;
			if (str[i + 1] && (str[i + 1] != 'n' || str[i] == '-'))
				return (false);
			while (str[i] == 'n')
				i++;
		}
		else
			return (false);
	}
	return (true);
}
