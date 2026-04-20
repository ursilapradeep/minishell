# Minishell Test Evaluation Sheet

## Signals
### Test Commands
1. **Interrupt Command Execution:**
   ```bash
   sleep 10
   # Press Ctrl+C during execution
   ```
   **Expected Result:** Command is interrupted, and a new prompt is displayed.

2. **Quit Command Execution:**
   ```bash
   sleep 10
   # Press Ctrl+\ during execution
   ```
   **Expected Result:** Command is terminated, and a new prompt is displayed.

3. **Interrupt at Prompt:**
   ```bash
   # Press Ctrl+C at the shell prompt
   ```
   **Expected Result:** New prompt is displayed without exiting.

4. **Quit at Prompt:**
   ```bash
   # Press Ctrl+\ at the shell prompt
   ```
   **Expected Result:** Signal is ignored.

## Double Quotes
### Test Commands
1. **Basic Double Quotes:**
   ```bash
   echo "hello world"
   ```
   **Expected Result:** `hello world` is printed.

2. **Variable Expansion:**
   ```bash
   echo "$HOME"
   ```
   **Expected Result:** Prints the value of the `HOME` variable.

3. **Unmatched Double Quotes:**
   ```bash
   echo "hello
   ```
   **Expected Result:** Syntax error is displayed.

## Logical Operators
### Test Commands
1. **Valid `&&` Usage:**
   ```bash
   ls && echo "done"
   ```
   **Expected Result:** `echo` runs only if `ls` succeeds.

2. **Valid `||` Usage:**
   ```bash
   ls || echo "failed"
   ```
   **Expected Result:** `echo` runs only if `ls` fails.

3. **Invalid `&&` Usage:**
   ```bash
   ls &&
   ```
   **Expected Result:** Syntax error is displayed.

4. **Invalid `||` Usage:**
   ```bash
   ls ||
   ```
   **Expected Result:** Syntax error is displayed.

## Pipes
### Test Commands
1. **Single Pipe:**
   ```bash
   ls | wc -l
   ```
   **Expected Result:** Output of `ls` is passed to `wc -l`.

2. **Multiple Pipes:**
   ```bash
   ls | grep "file" | wc -l
   ```
   **Expected Result:** Commands execute sequentially, passing output through the pipeline.

3. **Invalid Pipe Usage:**
   ```bash
   ls |
   ```
   **Expected Result:** Syntax error is displayed.

## Redirections
### Test Commands
1. **Output Redirection:**
   ```bash
   echo "hello" > file.txt
   ```
   **Expected Result:** `file.txt` contains `hello`.

2. **Input Redirection:**
   ```bash
   cat < file.txt
   ```
   **Expected Result:** Contents of `file.txt` are displayed.

3. **Invalid Redirection:**
   ```bash
   echo "hello" >
   ```
   **Expected Result:** Syntax error is displayed.

## Environment Variables
### Test Commands
1. **Print Variable:**
   ```bash
   echo $HOME
   ```
   **Expected Result:** Prints the value of `HOME`.

2. **Set Variable:**
   ```bash
   export VAR=value
   echo $VAR
   ```
   **Expected Result:** Prints `value`.

3. **Unset Variable:**
   ```bash
   unset VAR
   echo $VAR
   ```
   **Expected Result:** Prints an empty line.

4. **Undefined Variable:**
   ```bash
   echo $UNDEFINED
   ```
   **Expected Result:** Prints an empty line.

## Built-in Commands
### Test Commands
1. **Change Directory:**
   ```bash
   cd /tmp
   pwd
   ```
   **Expected Result:** Prints `/tmp`.

2. **Print Working Directory:**
   ```bash
   pwd
   ```
   **Expected Result:** Prints the current directory.

3. **Exit Shell:**
   ```bash
   exit
   ```
   **Expected Result:** Shell terminates.

4. **List Environment Variables:**
   ```bash
   env
   ```
   **Expected Result:** Prints all environment variables.

## Syntax Errors
### Test Commands
1. **Unmatched Parentheses:**
   ```bash
   echo (hello
   ```
   **Expected Result:** Syntax error is displayed.

2. **Unmatched Quotes:**
   ```bash
   echo "hello
   ```
   **Expected Result:** Syntax error is displayed.

3. **Standalone `&`:**
   ```bash
   &
   ```
   **Expected Result:** Syntax error is displayed.

## Edge Cases
### Test Commands
1. **Empty Command:**
   ```bash
   # Press Enter without typing anything
   ```
   **Expected Result:** New prompt is displayed without errors.

2. **Long Command:**
   ```bash
   echo "$(head -c 10000 < /dev/zero | tr '\0' 'a')"
   ```
   **Expected Result:** Command executes or displays an error if the length exceeds the limit.

3. **Special Characters:**
   ```bash
   echo "!@#$%^&*()"
   ```
   **Expected Result:** Prints `!@#$%^&*()`.

4. **Large Number of Arguments:**
   ```bash
   echo arg{1..1000}
   ```
   **Expected Result:** Command executes or displays an error if the argument limit is exceeded.