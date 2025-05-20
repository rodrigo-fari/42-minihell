/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:44:25 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/20 16:05:59 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hd_atributes(t_token *current)
{
	if(ft_strchr(current->value, '\"') ||
	ft_strchr(current->value, '\''))
		current->eof_inquote = true;
	if (ft_strchr(current->value, '$'))
		current->eof_envvar = true;
}
