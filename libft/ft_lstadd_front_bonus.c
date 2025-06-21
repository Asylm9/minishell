/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 00:53:01 by agaland           #+#    #+#             */
/*   Updated: 2024/10/29 19:09:57 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst && new)
	{
		new->next = *lst;
		*lst = new;
	}
}
/*
int	main(void)
{
	char	*s1 = ";jwd wfgu";
	char	*s2 = "add";

	t_list	*node1 = ft_lstnew(s1);
	t_list	*nodeadd = ft_lstnew(s2);
	t_list	*current = node1;

	while (current)
	{
		printf("%s\n", (char *) current->content);
		current = current->next;
	}

	ft_lstadd_front(&node1, nodeadd);
	current = node1;
	while (current)
	{
		printf("%s\n", (char *) current->content);
		current = current->next;
	}
}
*/
