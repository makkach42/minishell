#include "minishell.h"
void	*t_malloc(size_t i, int line, char *file)
{
	char *ptr;
	ptr = malloc(i);
	(void)line;
	(void)file;
	// printf("Memory allocation of address %p on line %d in file %s\n", ptr, line, file);
	return (ptr);
}
void	t_free(void *ptr, int line, char *file)
{
	(void)line;
	(void)file;
	// printf("Memory freeing of address %p on line %d in file %s\n", ptr, line, file);
	free(ptr);
}

char	*ft_strdup_leak(char *s1, int line)
{
	char *ptr;
	(void)line;
	// printf("Use of ft_strdup on line %d\n", line);
	ptr = ft_strdup(s1);
	return (ptr);
}

char	*ft_strjoin_leak(char *s1, char *s2, int line)
{
	char *ptr;
	(void)line;
	// printf("Use of ft_strjoin on line %d\n", line);
	ptr = ft_strjoin(s1, s2);
	return (ptr);
}

char	*ft_substr_leak(char *s, unsigned int start, size_t len, int line)
{
	char *ptr;
	(void)line;
	// printf("Use of ft_substr on line %d\n", line);
	ptr = ft_substr(s, start, len);
	return (ptr);

}
char	*ft_strtrim_leak(char *s1, char *set, int line)
{
	char *ptr;
	(void)line;
	ptr = ft_strtrim(s1, set);
	// printf("Use of ft_strtrim on line %d returning this address %p\n", line, ptr);
	return (ptr);
}

t_list *list_init_leak(char *str, int line)
{
	t_list *ptr;

	(void)line;
	ptr = list_init(str);
	// printf("Use of list_init on line %d returning this address %p\n", line, ptr);
	return (ptr);
}