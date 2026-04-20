#!/bin/bash

# Minishell Test Evaluation Script

# Signals
echo "Testing Signals..."
(sleep 10 &); sleep 1; kill -SIGINT $! # Interrupt Command Execution
(sleep 10 &); sleep 1; kill -SIGQUIT $! # Quit Command Execution

# Double Quotes
echo "Testing Double Quotes..."
echo "hello world" # Basic Double Quotes
echo "$HOME" # Variable Expansion
echo "hello # Unmatched Double Quotes (Expected: Syntax Error)"

# Logical Operators
echo "Testing Logical Operators..."
ls && echo "done" # Valid && Usage
ls || echo "failed" # Valid || Usage
ls && # Invalid && Usage (Expected: Syntax Error)
ls || # Invalid || Usage (Expected: Syntax Error)

# Pipes
echo "Testing Pipes..."
ls | wc -l # Single Pipe
ls | grep "file" | wc -l # Multiple Pipes
ls | # Invalid Pipe Usage (Expected: Syntax Error)

# Redirections
echo "Testing Redirections..."
echo "hello" > file.txt # Output Redirection
cat < file.txt # Input Redirection
echo "hello" > # Invalid Redirection (Expected: Syntax Error)

# Environment Variables
echo "Testing Environment Variables..."
echo $HOME # Print Variable
export VAR=value; echo $VAR # Set Variable
unset VAR; echo $VAR # Unset Variable
echo $UNDEFINED # Undefined Variable

# Built-in Commands
echo "Testing Built-in Commands..."
cd /tmp; pwd # Change Directory
pwd # Print Working Directory
exit # Exit Shell
env # List Environment Variables

# Syntax Errors
echo "Testing Syntax Errors..."
echo (hello # Unmatched Parentheses (Expected: Syntax Error)
echo "hello # Unmatched Quotes (Expected: Syntax Error)"
& # Standalone & (Expected: Syntax Error)

# Edge Cases
echo "Testing Edge Cases..."
# Empty Command (Press Enter without typing anything)
echo "$(head -c 10000 < /dev/zero | tr '\0' 'a')" # Long Command
echo "!@#$%^&*()" # Special Characters
echo arg{1..1000} # Large Number of Arguments