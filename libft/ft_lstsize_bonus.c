/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 01:26:19 by agaland           #+#    #+#             */
/*   Updated: 2024/11/01 20:01:33 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	counter;

	counter = 0;
	while (lst != NULL)
	{
		counter++;
		lst = lst->next;
	}
	return (counter);
}
/*
int	main(void)
{
	char	c1 = 'a';
	char	c2 = 'b';
	char	c3 = 'c';
	t_list	*node1 = ft_lstnew(&c1);
	t_list	*node2 = ft_lstnew(&c2);
	t_list	*node3 = ft_lstnew(&c3);
	int		lstlen;

	node1->next = node2;
	node2->next = node3;
	lstlen = ft_lstsize(node1);
	printf("%d\n", lstlen);
}
*/
