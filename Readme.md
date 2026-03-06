Overview of the project 

1. Input (Readline)
Goal: Capture the raw string.

Key Function: readline().

Action: It waits for the user, takes the string, and adds it to the history.

2. Lexer / Tokenizer
Goal: Break the string into "words" (tokens).

Result: A list of strings or tokens.

Example: ls -l | grep ".c" becomes ["ls", "-l", "|", "grep", "\".c\""].

3. Parser
Goal: Organize tokens into a logical structure.

Result: A Command Table (often a linked list of structs).

Action: It identifies which tokens are commands, which are arguments, and which are operators like pipes (|) or redirections (>).

4. Expander
Goal: Resolve shell "magic."

Action:

Environment Variables: Changes $USER to john.

Quotes: Removes outer quotes ("hello" becomes hello).

Wildcards: (Bonus) Changes *.c into a list of filenames.

5. Executor
Goal: Turn the structure into a running process.

Action:

Built-in Check: If it's cd or exit, it runs in the Parent.

Forking: If it's ls or grep, it creates a Child.

Redirection: Uses dup2() to swap standard input/output with files.

Path Finding: Finds where the binary lives (e.g., /bin/ls).
https://man7.org/linux/man-pages/man3/readline.3.html