/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 09:47:50 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/24 18:59:49 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;
//CTRL C NO HEARDOC SHOULD QUIT
int	main(int ac, char **av, char **envp)
{
	t_env		*env;
	char		*input;

	if (ac != 1 || av[1] || ft_array_size(envp) < 10)//! Verificar se isto e permitido pelo subject
	{
		write(2, "[Error] Invalid program usage\n", 30);
		return (1);
	}
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
			free_env_list(env);
			break ;
		}
		add_history(input);
		env = get_env(NULL);
		ms_exec(input, env);
	}
	return (0);
}
