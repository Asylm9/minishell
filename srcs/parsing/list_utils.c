/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 21:11:07 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/12 21:26:19 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	create_token_list(t_lst **tok_lst)
{
	(*tok_lst) = malloc(sizeof(t_lst));
	if (!(*tok_lst))
	{
		perror("Token list malloc failed.\n");
		return (ERROR);
	}
	(*tok_lst)->value = NULL;
	(*tok_lst)->expand = NO_EXPAND;
	(*tok_lst)->type = WORD;
	(*tok_lst)->next = NULL;
	return (SUCCESS);
}

int	create_token_node(t_lst **tok_lst)
{
	t_lst	*new_token;
	t_lst	*temp;

	temp = *tok_lst;
	new_token = malloc(sizeof(t_lst));
	if (!new_token)
	{
		perror("Token list malloc failed.\n");
		return (ERROR);
	}
	new_token->value = NULL;
	new_token->expand = NO_EXPAND;
	new_token->type = WORD;
	new_token->next = NULL;
	new_token->hd_fd = -1;
	if (*tok_lst == NULL)
		*tok_lst = new_token;
	else
	{
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
	}
	return (SUCCESS);
}

int	skip_spaces(const char *input, int *pos)
{
	while (input[*pos] && input[*pos] == ' ')
		(*pos)++;
	if (input[*pos] == '\0')
		return (ERROR);
	return (SUCCESS);
}