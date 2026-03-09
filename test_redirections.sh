#!/bin/bash

# Minishell Redirections Test Script
# This script provides test cases for the redirection implementation

echo "=== Testing Minishell Redirections ==="
echo ""

# Test 1: Output Redirection (>)
echo "Test 1: Output redirection (>)"
echo "Command: echo 'Hello World' > test_output.txt"
echo ""

# Test 2: Append Redirection (>>)
echo "Test 2: Append redirection (>>)"
echo "Command: echo 'Line 1' > test_append.txt"
echo "Command: echo 'Line 2' >> test_append.txt"
echo "Command: cat test_append.txt"
echo ""

# Test 3: Input Redirection (<)
echo "Test 3: Input redirection (<)"
echo "Command: echo 'Test content' > test_input.txt"
echo "Command: cat < test_input.txt"
echo ""

# Test 4: Heredoc (<<)
echo "Test 4: Heredoc (<<)"
echo "Command: cat << EOF"
echo "Then type:"
echo "  Line 1"
echo "  Line 2"
echo "  EOF"
echo ""

# Test 5: Multiple redirections
echo "Test 5: Multiple redirections"
echo "Command: cat < test_input.txt > test_copy.txt"
echo "Command: cat test_copy.txt"
echo ""

# Test 6: Append with commands
echo "Test 6: Append with ls"
echo "Command: ls -la >> list.txt"
echo "Command: cat list.txt"
echo ""

# Test 7: Error cases
echo "Test 7: Error handling"
echo "Command: cat < nonexistent.txt (should show error)"
echo ""

# Test 8: With pipes
echo "Test 8: Redirections with pipes"
echo "Command: echo 'test' | cat > piped_output.txt"
echo "Command: cat piped_output.txt"
echo ""

echo "=== Manual Testing Instructions ==="
echo "1. Run ./minishell"
echo "2. Try each command above"
echo "3. Verify output matches expected behavior"
echo "4. Check files are created with correct permissions (644)"
echo "5. Ensure no memory leaks with: leaks minishell"
echo ""

# Create test files for demonstration
echo "Creating test files..."
echo "This is test input" > test_input_demo.txt
echo "Test file created: test_input_demo.txt"
echo ""

echo "You can now test with:"
echo "  ./minishell"
echo "  minishell$ cat < test_input_demo.txt"
echo "  minishell$ echo 'output' > my_output.txt"
echo "  minishell$ cat my_output.txt"
