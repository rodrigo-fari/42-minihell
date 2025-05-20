/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps_expand_variable.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:34:15 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/20 18:44:12 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*var_expand(char *input)
{
	t_env	*tmp;

	tmp = get_env(NULL);
	while (tmp)
	{
		if (ft_strcmp(input, tmp->key) == 0)
			return (ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (ft_strdup(""));
}

//Criar funcao para ver quantos $ tem no input

//Criar funcao que faz o split do input pelo $ (' ' && '$')
//		expande individualmente com var_exoand(recebe string)
//		junta tudo em uma linha so linha
//		devolve linha completa expandida.