/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:18:50 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/30 17:57:26 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigpipe(struct sigaction *sa_ignore, struct sigaction *sa_old)
{
	sa_ignore->sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore->sa_mask);
	sa_ignore->sa_flags = 0;
	sigaction(SIGPIPE, sa_ignore, sa_old);
}

bool	process_flags(t_token **tmp, bool *flag, struct sigaction *sa_old)
{
	while (*tmp && flag_verify((*tmp)->value))
	{
		*flag = false;
		if (!(*tmp)->next)
		{
			sigaction(SIGPIPE, sa_old, NULL);
			return (false);
		}
		*tmp = (*tmp)->next;
	}
	return (true);
}

void	print_tokens(t_token *tmp)
{
	while (tmp)
	{
		if (tmp->value[0] != '\0')
		{
			ms_print_fd(tmp->value, 1);
			if (tmp->next)
				ms_print_fd(" ", 1);
		}
		tmp = tmp->next;
	}
}

void	bi_echo(t_token *tmp)
{
	bool				flag;
	struct sigaction	sa_old;
	struct sigaction	sa_ignore;

	handle_sigpipe(&sa_ignore, &sa_old);
	flag = true;
	if (!tmp->value[0])
	{
		write(STDOUT_FILENO, "\n", 1);
		sigaction(SIGPIPE, &sa_old, NULL);
		return ;
	}
	tmp = tmp->next;
	if (!process_flags(&tmp, &flag, &sa_old))
		return ;
	print_tokens(tmp);
	if (!flag)
		return ;
	else
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
