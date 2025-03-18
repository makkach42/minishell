#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

int countstrings(char *str)
{
	int i = 0;
	int count = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			count++;
		i++;
	}	
	return (count / 2);
}

int	*where_string_starts(char *str)
{
	int i;
	int j;
	int	flag;
	int	how_many_strings;
	int *arr;

	how_many_strings = countstrings(str);
	i = -1;
	j = 0;
	flag = 0;
	arr = malloc(sizeof(int) * how_many_strings);
	while (how_many_strings)
	{
		while (str[++i] != '\"'){}
		if (str[i] == '\0')
			return (NULL);
		if (flag == 0 && str[i] == '\"')
		{
			arr[j] = i;
			j++;
            i++;
			while (str && str[i] && str[i] != '\"')
				i++;
			how_many_strings--;
		}
	}
	return (arr);
}

int main(void)
{
    int i = 0;
    char *str = readline("$");
    int *arr = where_string_starts(str);
    while (i < 2)
    {
        printf("%d\n", arr[i]);
        i++;
    }
    return 0;
}
