/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:21:31 by agaland           #+#    #+#             */
/*   Updated: 2024/11/01 19:31:54 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
	{
		lst = lst->next;
	}
	return (lst);
}

/*
int	main(void)
{
	int	x1 = 5;
	int	x2 = 3;
	int x3 = 10;
	int	x4 = 7;
	t_list	*node1 = ft_lstnew(&x1);
	t_list	*node2 = ft_lstnew(&x2);
	t_list	*node3 = ft_lstnew(&x3);
	t_list	*node4 = ft_lstnew(&x4);
	t_list	*lastnode;

	node1->next = node2;
	node2->next = node3;
	node3->next = node4;
	lastnode = ft_lstlast(node1);

	printf("%d\n", * (int *) lastnode->content);
}
*/
