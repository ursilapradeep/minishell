/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uvadakku <uvadakku@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:47:26 by uvadakku          #+#    #+#             */
/*   Updated: 2026/03/17 18:27:46 by uvadakku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h" // Include minishell declarations and prototypes.

int is_space(char c) // Check whether a character is considered whitespace.
{
    return (c == ' ' || c == '\t' || c == '\n'); // Return true for space, tab, or newline.
}

int	word_end(char *input, int i) // Find where the current token ends.
{
    char	quote; // Track active quote type while scanning.

    quote = '\0'; // Start outside of quotes.
    while (input[i]) // Scan until end of input string.
    {
         if (!quote && (input[i] == '\'' || input[i] == '"')) // Enter quote context when quote opens.
            quote = input[i]; // Save current quote delimiter.
        else if (quote && input[i] == quote) // Detect matching closing quote.
            quote = '\0'; // Exit quote context.
        else if (!quote && is_space(input[i])) // Stop at unquoted whitespace.
            break ; // Token boundary reached.
        i++; // Move to next character.
    }
    return (i); // Return index of token end.
}

int	word_len_without_quotes(char *input, int start, int end) // Compute token length excluding quote chars.
{
    int		len; // Count of copied non-quote characters.
    int		i; // Current scan index.
    char	quote; // Active quote tracker.

    len = 0; // Initialize resulting length.
    i = start; // Start scanning at token start.
    quote = '\0'; // Initially not inside quotes.
    while (i < end) // Iterate only through token range.
    {
        if (!quote && (input[i] == '\'' || input[i] == '"')) // Opening quote found.
            quote = input[i]; // Enter quote mode.
        else if (quote && input[i] == quote) // Closing matching quote found.
            quote = '\0'; // Exit quote mode.
        else // Regular character that should remain.
            len++; // Increase visible length.
        i++; // Advance to next character.
    }
    return (len); // Return final length without quotes.
}

void copy_without_quotes(char *dst, char *input, int start, int end) // Copy token content while stripping quote chars.
{
    int		j; // Destination index.
    int		i; // Source index.
    char	quote; // Active quote tracker.

    j = 0; // Start writing at beginning of destination.
    i = start; // Start reading at token start.
    quote = '\0'; // Initially outside quotes.
    while (i < end) // Walk through source token range.
    {
        if (!quote && (input[i] == '\'' || input[i] == '"')) // Opening quote encountered.
            quote = input[i]; // Enter quote mode.
        else if (quote && input[i] == quote) // Closing quote encountered.
            quote = '\0'; // Exit quote mode.
        else // Normal content character.
            dst[j++] = input[i]; // Copy to destination and advance output index.
        i++; // Advance input index.
    }
    dst[j] = '\0'; // Null-terminate destination string.
}
