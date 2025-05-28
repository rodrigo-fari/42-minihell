/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:39:51 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/28 03:12:48 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	bi_pwd(void)
{
	char	*cwd;
	char	*env_pwd;

	cwd = NULL;
	env_pwd = get_own_env("PWD");
	if (env_pwd && *env_pwd)
	{
		printf("%s\n", env_pwd);
		return ;
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = get_current_directory(NULL);
	printf("%s\n", cwd);
	if (cwd)
		free(cwd);
}

char	*get_current_directory(char *current_wd)
{
	static char	*static_pwd;
	char		*tmp;

	if (current_wd)
	{
		tmp = ft_strdup(current_wd);
		static_pwd = tmp;
		free(tmp);
	}
	return (static_pwd);
}
