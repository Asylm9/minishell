/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:25:27 by agaland           #+#    #+#             */
/*   Updated: 2024/11/01 19:14:16 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*current;

	if (!lst)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
	{
		current = current->next;
	}
	current->next = new;
}
/*
int	main(void)
{
	char	*s1 = ";jwd wfgu";
	char	*s11 = "zbem";
	char	*s2 = "add";

	t_list	*node1 = ft_lstnew(s1);
	t_list	*node2 = ft_lstnew(s11);
	t_list	*nodeadd = ft_lstnew(s2);
	t_list	*current = node1;

	while (current)
	{
		printf("%s\n", (char *) current->content);
		current = current->next;
	}

	ft_lstadd_back(&node1, node2);
	ft_lstadd_back(&node1, nodeadd);

	current = node1;
	while (current)
	{
		printf("%s\n", (char *) current->content);
		current = current->next;
	}
}
*/
