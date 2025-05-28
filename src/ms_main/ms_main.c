/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 09:47:50 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 17:00:42 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

int	main(int ac, char **av, char **envp)
{
	t_env		*env;
	char		*input;

	if (ac != 1 || av[1])
		return (1);
	env = env_to_struct(envp);
	get_env(env);
	shlvl_warning();
	while (true)
	{
		signal(SIGINT, sig_ctrl_c);
		signal(SIGPIPE, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		env = get_env(NULL);
		input = readline(RESET"[42]Minihell$ ");
		if (!input)
		{
			write(1, "\n", 1);
			break ;
		}
		add_history(input);
		ms_exec(input, env);
	}
	cc_shell(get_shell(), true, true, false);
	return (0);
}
