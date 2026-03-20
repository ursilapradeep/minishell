/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spaipur- <<spaipur-@student.42.fr>>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 15:00:00 by spaipur-          #+#    #+#             */
/*   Updated: 2026/03/20 17:04:54 by spaipur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <string.h>

/* Test counters */
static int g_pass = 0;
static int g_fail = 0;

/* Helper macro to test conditions */
#define TEST(name, condition) \
	do { \
		if (condition) { \
			printf("[PASS] %s\n", name); \
			g_pass++; \
		} else { \
			printf("[FAIL] %s\n", name); \
			g_fail++; \
		} \
	} while (0)

/* ========== TEST: skip_whitespace ========== */
void test_skip_whitespace(void)
{
	const char *result;
	
	printf("\n--- Testing skip_whitespace ---\n");
	
	/* Test 1: Skip leading spaces */
	result = skip_whitespace("   hello");
	TEST("skip_whitespace: skip leading spaces", 
		result != NULL && strcmp(result, "hello") == 0);
	
	/* Test 2: Skip tabs */
	result = skip_whitespace("\t\t\tworld");
	TEST("skip_whitespace: skip tabs", 
		result != NULL && strcmp(result, "world") == 0);
	
	/* Test 3: Mixed spaces and tabs */
	result = skip_whitespace("  \t  test");
	TEST("skip_whitespace: skip mixed whitespace", 
		result != NULL && strcmp(result, "test") == 0);
	
	/* Test 4: No whitespace */
	result = skip_whitespace("hello");
	TEST("skip_whitespace: no leading whitespace", 
		result != NULL && strcmp(result, "hello") == 0);
	
	/* Test 5: Empty string */
	result = skip_whitespace("");
	TEST("skip_whitespace: empty string returns NULL", result == NULL);
	
	/* Test 6: Only whitespace */
	result = skip_whitespace("   ");
	TEST("skip_whitespace: only whitespace returns NULL", result == NULL);
	
	/* Test 7: NULL input */
	result = skip_whitespace(NULL);
	TEST("skip_whitespace: NULL input returns NULL", result == NULL);
}

/* ========== TEST: is_redirect_operator ========== */
void test_is_redirect_operator(void)
{
	t_token_type result;
	
	printf("\n--- Testing is_redirect_operator ---\n");
	
	/* Test 1: Output redirect > */
	result = is_redirect_operator(">");
	TEST("is_redirect_operator: '>' returns TOKEN_REDIRECT_OUT", 
		result == TOKEN_REDIRECT_OUT);
	
	/* Test 2: Input redirect < */
	result = is_redirect_operator("<");
	TEST("is_redirect_operator: '<' returns TOKEN_REDIRECT_IN", 
		result == TOKEN_REDIRECT_IN);
	
	/* Test 3: Append redirect >> */
	result = is_redirect_operator(">>");
	TEST("is_redirect_operator: '>>' returns TOKEN_REDIRECT_APPEND", 
		result == TOKEN_REDIRECT_APPEND);
	
	/* Test 4: Heredoc << */
	result = is_redirect_operator("<<");
	TEST("is_redirect_operator: '<<' returns TOKEN_HEREDOC", 
		result == TOKEN_HEREDOC);
	
	/* Test 5: Invalid operator */
	result = is_redirect_operator("invalid");
	TEST("is_redirect_operator: 'invalid' returns 0", result == 0);
	
	/* Test 6: Empty string */
	result = is_redirect_operator("");
	TEST("is_redirect_operator: empty string returns 0", result == 0);
	
	/* Test 7: NULL input */
	result = is_redirect_operator(NULL);
	TEST("is_redirect_operator: NULL returns 0", result == 0);
	
	/* Test 8: > at start of string with text after */
	result = is_redirect_operator("> file.txt");
	TEST("is_redirect_operator: '> file.txt' returns TOKEN_REDIRECT_OUT", 
		result == TOKEN_REDIRECT_OUT);
}

/* ========== TEST: extract_redirect_filename ========== */
void test_extract_redirect_filename(void)
{
	char *result;
	
	printf("\n--- Testing extract_redirect_filename ---\n");
	
	/* Test 1: Simple filename */
	result = extract_redirect_filename(" file.txt");
	TEST("extract_redirect_filename: extracts 'file.txt'", 
		result != NULL && strcmp(result, "file.txt") == 0);
	if (result) free(result);
	
	/* Test 2: Filename with spaces before */
	result = extract_redirect_filename("   output.log");
	TEST("extract_redirect_filename: skips spaces before filename", 
		result != NULL && strcmp(result, "output.log") == 0);
	if (result) free(result);
	
	/* Test 3: Filename stops at pipe */
	result = extract_redirect_filename(" file.txt | grep");
	TEST("extract_redirect_filename: stops at pipe", 
		result != NULL && strcmp(result, "file.txt") == 0);
	if (result) free(result);
	
	/* Test 4: Filename stops at redirect operator */
	result = extract_redirect_filename(" file1.txt > file2.txt");
	TEST("extract_redirect_filename: stops at redirect operator", 
		result != NULL && strcmp(result, "file1.txt") == 0);
	if (result) free(result);
	
	/* Test 5: Empty string after spaces (error case) */
	result = extract_redirect_filename("   ");
	TEST("extract_redirect_filename: empty input returns NULL", result == NULL);
	if (result) free(result);
	
	/* Test 6: NULL input */
	result = extract_redirect_filename(NULL);
	TEST("extract_redirect_filename: NULL input returns NULL", result == NULL);
	
	/* Test 7: Path with directory */
	result = extract_redirect_filename(" /tmp/test.txt");
	TEST("extract_redirect_filename: extracts path '/tmp/test.txt'", 
		result != NULL && strcmp(result, "/tmp/test.txt") == 0);
	if (result) free(result);
}

/* ========== MAIN TEST RUNNER ========== */
int main(void)
{
	printf("==================================================\n");
	printf("       MINISHELL PARSER UNIT TESTS\n");
	printf("==================================================\n");
	
	test_skip_whitespace();
	test_is_redirect_operator();
	test_extract_redirect_filename();
	
	printf("\n==================================================\n");
	printf("  PASSED: %d\n", g_pass);
	printf("  FAILED: %d\n", g_fail);
	printf("  TOTAL:  %d\n", g_pass + g_fail);
	printf("==================================================\n");
	
	return (g_fail == 0 ? 0 : 1);
}
