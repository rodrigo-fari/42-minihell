/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps_quotes_and_expand_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-fari <rde-fari@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:50:29 by rde-fari          #+#    #+#             */
/*   Updated: 2025/05/30 18:18:03 by rde-fari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*remove_quotes_and_expand(char *input, t_env *env)
// {
// 	int		i;
// 	char	quote;
// 	char	*ret_str;
// 	char	*tmp;

// 	i = 0;
// 	quote = '\0';
// 	ret_str = NULL;
// 	env = get_env(NULL);
// 	while (input[i])
// 	{
// 		if ((input[i] == '\'' || input[i] == '\"') && quote == '\0')
// 			quote = input[i++];
// 		else if (input[i] == quote)
// 		{
// 			quote = '\0';
// 			i++;
// 		}
// 		else if (input[i] == '$' && quote != '\'')
// 		{
// 			if (input[i + 1] == '?')
// 				append_exit_status(&ret_str, &i);
// 			else if (!input[i + 1] || (!ft_isalnum(input[i + 1])
// 					&& input[i + 1] != '_'))
// 			{
// 				tmp = append_char_to_string(ret_str, '$');
// 				if (ret_str)
// 					free(ret_str);
// 				ret_str = tmp;
// 				i++;
// 			}
// 			else
// 				append_variable(&ret_str, input, &i, env);
// 		}
// 		else
// 		{
// 			tmp = append_char_to_string(ret_str, input[i]);
// 			if (ret_str)
// 				free(ret_str);
// 			ret_str = tmp;
// 			i++;
// 		}
// 	}
// 	return (ret_str);
// }

char	*remove_quotes(char *input)
{
	char	*ret_str;
	char	*tmp;
	int		i;

	i = 0;
	ret_str = NULL;
	while (input[i])
	{
		while (input[i] && (input[i] == '\"' || input[i] == '\''))
			i++;
		while (input[i] && input[i] != '\"' && input[i] != '\'')
		{
			tmp = append_char_to_string(ret_str, input[i]);
			if (ret_str)
				free(ret_str);
			ret_str = tmp;
			i++;
		}
		i++;
	}
	free(tmp);
	return (ret_str);
}
