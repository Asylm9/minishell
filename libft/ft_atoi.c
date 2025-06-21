/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:16:52 by agaland           #+#    #+#             */
/*   Updated: 2024/11/03 00:57:30 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_init(size_t *i, long *result, long *test, int *sign)
{
	*i = 0;
	*result = 0;
	*test = 0;
	*sign = 1;
}

int	ft_atoi(const char *str)
{
	size_t	i;
	long	result;
	long	test;
	int		sign;

	ft_init(&i, &result, &test, &sign);
	while ((str[i] >= 9 && str[i] <= 13) || (str[i] == 32))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		if (test > result && sign == 1)
			return (-1);
		else if (test > result && sign == -1)
			return (0);
		test = result;
		i++;
	}
	return (result * sign);
}
/*
int	main(void)
{
	printf("FT_ATOI:\n");
	//ft_atoi(0);
	printf("%d\n", ft_atoi(""));
	printf("%d\n", ft_atoi("   +324156sefg"));
	printf("%d\n", ft_atoi("  -+-45"));	
	printf("%d\n", ft_atoi("92233720368580788888880000000"));	
	printf("%d\n", ft_atoi("-92233720368580788888880000000"));
	printf("%d\n", ft_atoi("  -3151"));
	printf("%d\n", ft_atoi("2147483647"));
	printf("%d\n\n", ft_atoi("-2147483648"));
	
	
	printf("%d\n", ft_atoi("2147483649"));
	printf("%d\n", ft_atoi("9223372036854775807"));
	printf("%d\n", ft_atoi("92233720368"));

	//atoi(0);
	printf("ATOI:\n");
	printf("%d\n", atoi(""));
	printf("%d\n", atoi("   +324156sefg"));
	printf("%d\n", atoi("  -+-45"));	
	printf("%d\n", atoi("92233720368580788888880000000"));	
	printf("%d\n", atoi("-92233720368580788888880000000"));
	printf("%d\n", atoi("  -3151"));
	printf("%d\n", atoi("2147483647"));
	printf("%d\n\n", atoi("-2147483648"));
	

	printf("%d\n", atoi("2147483649"));
	printf("%d\n", atoi("9223372036854775807"));
	printf("%d\n", atoi("92233720368"));
	
}
*/
