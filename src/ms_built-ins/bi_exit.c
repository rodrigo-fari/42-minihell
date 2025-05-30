/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 19:13:25 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/31 12:32:30 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_exit(t_token *token, char **new_commands)
{
	if (new_commands)
		free_splits(new_commands);
	ft_putendl_fd("exit", 1);
	if (tk_listsize(token) == 1)
		define_exit_status(NULL);
	else if (!check_exit_signals(token, 0, 0))
		define_exit_status(NULL);
	else
	{
		if (!check_exit_arguments(token))
			return ;
	}
	define_exit_status(token->next->value);
}

void	define_exit_status(char *exit_status)
{
	long		exit_lnumber;

	if (!exit_status)
	{
		cc_shell(get_shell(), true, true, false);
		exit(g_exit_status);
	}
	if (ft_atol(exit_status) != ft_atod(exit_status))
	{
		g_exit_status = 2;
		cc_shell(get_shell(), true, true, false);
		exit (g_exit_status);
	}
	exit_lnumber = ft_atol(exit_status);
	if (exit_lnumber > 256 || exit_lnumber < 0)
		exit_lnumber %= 256;
	g_exit_status = exit_lnumber;
	cc_shell(get_shell(), true, true, false);
	exit (g_exit_status);
}

bool	check_exit_arguments(t_token *token)
{
	if (tk_listsize(token) > 2)
	{
		print_error("Minishell: exit: too many arguments");
		g_exit_status = 1;
		return (false);
	}
	if (tk_listsize(token) == 2 && ft_strcmp(token->next->value, "--") == 0)
	{
		g_exit_status = 2;
		return (false);
	}
	return (true);
}

bool	check_exit_signals(t_token *token, int i, int qnt)
{
	if (!token || !token->next || !token->next->value)
		return (false);
	while (token->next->value[i])
	{
		while (token->next->value[i] && (!ft_isdigit(token->next->value[i])))
		{
			qnt++;
			i++;
		}
		if (!check_signal_quantity(qnt))
			return (false);
		while (token->next->value[i] && (ft_isdigit(token->next->value[i])))
			i++;
		if (token->next->value[i])
		{
			print_error("Minishell: exit: numeric argument required");
			g_exit_status = 2;
			return (false);
		}
	}
	return (true);
}

bool	check_signal_quantity(int qnt)
{
	if (qnt > 1)
	{
		print_error("Minishell: exit: Invalid argument");
		g_exit_status = 2;
		return (false);
	}
	return (true);
}
