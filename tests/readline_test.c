#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("blablabla> ");
		if (!input)
		{
			printf("exit\n");
			break;
		}

        if (*input)
            add_history(input);
		printf("%s\n", input);

	free(input);
	}
	return 0;
}
