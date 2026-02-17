#include "../minishell.h"

static int is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}
static int count_args(char *input)
{
    int count;
    int i; // Index to traverse input

    count = 0;
    i = 0;
    while (input[i])
    {
        while (input[i] &&  is_space(input[i]))
            i++; // Skip leading spaces
        if (input[i])
        {
            count++; // Found an argument
            while (input[i] && !is_space(input[i])) // Skip non-space characters
                i++;
        }
    }
    return (count);
}
/*Input string: "ls  -la /tmp"
    Process:
    Skip spaces → find "ls" → copy into args[0]
    Skip spaces → find "-la" → copy into args[1]
    Skip spaces → find "/tmp" → copy into args[2]
    Set args[3] = NULL */
static int	add_word(char **args, char *input, int start, int index)
{
	int	len;

	len = 0;
	while (input[start + len] && !is_space(input[start + len]))
		len++;
	args[index] = malloc(len + 1); 
	if (!args[index])
    {
        free_args(args); // Free previously allocated memory on failure
        return (0);
    }
	ft_strlcpy(args[index], input + start, len + 1); // Copy word into args array
	return (1);
}

static int	fill_args(char **args, char *input)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++; // Skip leading spaces
		if (input[i])
		{
			if (!add_word(args, input, i, j)) // Add word to args array
				return (0);
			while (input[i] && !is_space(input[i]))
				i++;
			j++;
		}
	}
	args[j] = NULL;
	return (1);
}

// Split input into arguments
char **split_args(char *input)
{
    char **args;
    char *copy;
    int count;

    if (!input)
        return (NULL);
    copy = ft_strdup(input); // Duplicate input to avoid modifying original string
    if (!copy)
        return (NULL);
    count = count_args(copy); // Count number of arguments to allocate array
    args = malloc((sizeof(char *) * (count + 1))); // +1 for NULL terminator
    if (!args)
    {
        free(copy);
        return (NULL);
    }
    if (!fill_args(args, copy))
    {
        free(copy);
        free_args(args);
        return (NULL);
    }
    free(copy);
    return (args);
}

// Free arguments array
void free_args(char **args)
{
    int i;

    if (!args)
        return ;
    i = 0;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}