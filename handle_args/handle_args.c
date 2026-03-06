#include "../minishell.h"

static int is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

static int	word_end(char *input, int i)
{
    char	quote;

    quote = '\0';
    while (input[i])
    {
        if (!quote && (input[i] == '\'' || input[i] == '"'))
            quote = input[i];
        else if (quote && input[i] == quote)
            quote = '\0';
        else if (!quote && is_space(input[i]))
            break ;
        i++;
    }
    return (i);
}

static int	word_len_without_quotes(char *input, int start, int end)
{
    int		len;
    int		i;
    char	quote;

    len = 0;
    i = start;
    quote = '\0';
    while (i < end)
    {
        if (!quote && (input[i] == '\'' || input[i] == '"'))
            quote = input[i];
        else if (quote && input[i] == quote)
            quote = '\0';
        else
            len++;
        i++;
    }
    return (len);
}

static void	copy_without_quotes(char *dst, char *input, int start, int end)
{
    int		j;
    int		i;
    char	quote;

    j = 0;
    i = start;
    quote = '\0';
    while (i < end)
    {
        if (!quote && (input[i] == '\'' || input[i] == '"'))
            quote = input[i];
        else if (quote && input[i] == quote)
            quote = '\0';
        else
            dst[j++] = input[i];
        i++;
    }
    dst[j] = '\0';
}

static int count_args(char *input)
{
    int count;
    int i; // Index to traverse input
    int end;

    count = 0;
    i = 0;
    while (input[i])
    {
        while (input[i] &&  is_space(input[i]))
            i++; // Skip leading spaces
        if (input[i])
        {
            count++; // Found an argument
            end = word_end(input, i);
            i = end;
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
static int add_word(char **args, char *input, int start,int end, int index)
{
    int end;
	int	len;

    len = word_len_without_quotes(input, start, end);
	args[index] = malloc(len + 1); 
	if (!args[index])
    {
        free_args(args); // Free previously allocated memory on failure
        return (0);
    }
    copy_without_quotes(args[index], input, start, end);
	return (1);
}

static int fill_args(char **args, char *input)
{
	int	i;
	int	j;
    int	end;

	i = 0;
	j = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++; // Skip leading spaces
		if (input[i])
		{
            end = word_end(input, i);
            if (!add_word(args, input, i, end, j)) // Add word to args array
				return (0);
            i = end;
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
