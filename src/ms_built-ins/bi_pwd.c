/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:39:51 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/24 17:25:51 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	bi_pwd(void)
{
	char *cwd = NULL;
	char *env_pwd = get_own_env("PWD");

	if (!get_own_env("PWD") || !get_own_env("OLDPWD"))
	{
		printf("%s\n", get_current_directory(NULL));
		return ;
	}
	if (env_pwd && *env_pwd)
	{
		printf("%s\n", env_pwd);
		return;
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: pwd");
		g_exit_status = 2;
		return;
	}
	printf("%s\n", cwd);
	free(cwd);
}

char	*get_current_directory(char *current_wd)
{
	static char	*static_pwd;

	if (current_wd)
		static_pwd = current_wd;
	return (static_pwd);
}
