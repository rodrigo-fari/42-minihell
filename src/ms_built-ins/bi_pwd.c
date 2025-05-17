/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 15:39:51 by rde-fari          #+#    #+#             */
/*   Updated: 2025/04/21 22:01:10 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	bi_pwd(void)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		bi_error("Minishell: pwd: Invalid path");
		g_exit_status = 2;
		return ;
	}
	printf("%s\n", cwd);
	free (cwd);
}
