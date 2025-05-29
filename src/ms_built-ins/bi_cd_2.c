/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:50:15 by aeberius          #+#    #+#             */
/*   Updated: 2025/05/29 02:23:58 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_cd_error(void)
{
	print_error("Minishell: cd: No such file or directory");
	g_exit_status = 1;
}

void	change_directory(const char *path, t_env *env, char *old_pwd)
{
	if (!path)
	{
		g_exit_status = 1;
		return ;
	}
	if (chdir(path) == 0)
		update_pwd(env, old_pwd);
	else
		handle_cd_error();
}
