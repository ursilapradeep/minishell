*This project has been created as part of the 42 curriculum by uvadakku, spaipur-.*

# Minishell

## Description
Minishell is a small Unix shell written in C. The goal is to reproduce core Bash-like behavior for command execution, pipes, redirections, environment expansion, and signal handling.

Project flow overview:

1. Input (Readline)
	- Goal: capture raw user input.
	- Key function: `readline()`.
	- Action: waits for user input and stores command history.

2. Lexer / Tokenizer
	- Goal: split the input into tokens.
	- Result: token list (words/operators).
	- Example: `ls -l | grep ".c"` → `["ls", "-l", "|", "grep", "\".c\""]`.

3. Parser
	- Goal: organize tokens into command structures.
	- Result: a command table / linked command list.
	- Action: identifies commands, arguments, pipes, and redirections.

4. Expander
	- Goal: resolve shell expansions.
	- Action:
	  - Environment variables: `$USER` → user value.
	  - Quotes handling.
	  - Wildcards (`*`) are part of bonus scope.

5. Executor
	- Goal: execute parsed commands.
	- Action:
	  - Builtins run in parent context when required.
	  - External commands run through fork/exec.
	  - Redirections use `dup2()`.
	  - Command lookup uses `PATH` (or relative/absolute paths).

## Instructions

### Requirements
- Linux/macOS
- `cc`
- `make`
- `readline` development library

### Build
```bash
make
```

### Run
```bash
./minishell
```

### Clean
```bash
make clean
make fclean
```

## Resources

### Technical references
- Bash Reference Manual:https://www.gnu.org/software/bash/manual/bash.html
- Bash (unix Shell):https://en.wikipedia.org/wiki/Bash_(Unix_shell)
- Readline manual: https://man7.org/linux/man-pages/man3/readline.3.html
- Redirections (pipex background): https://42-cursus.gitbook.io/guide/2-rank-02/pipex/understand-pipex
- Heredoc reference: https://linuxize.com/post/bash-heredoc/
- Signal basics: https://medium.com/@shyamkk/understanding-the-commands-behind-ctrl-c-and-ctrl-z-interrupt-signals-in-linux-3b8c9ec09708

### AI usage disclosure
AI tools were used for:
- clarifying C/system-call concepts (signals, pipes, redirections),
- brainstorming test cases,
- reviewing code structure and refactoring ideas,
- improving documentation wording.

All generated suggestions were manually reviewed, tested, and adapted before integration.

# 1) backslash escape
printf 'echo hello\\ world\n' | ./minishell; echo "minishell=$?"
printf 'echo hello\\ world\n' | bash;       echo "bash=$?"

# 2) invalid leading &&
printf '&& ls\n' | ./minishell; echo "minishell=$?"
printf '&& ls\n' | bash;       echo "bash=$?"

# 3) input redirection
printf 'foo\nbar\n' > in.txt
printf 'cat < in.txt\n' | ./minishell; echo "minishell=$?"
printf 'cat < in.txt\n' | bash;       echo "bash=$?"

# 4) pipe + and/or chain
printf 'ls | grep foo && echo found || echo not found\n' | ./minishell; echo "minishell=$?"
printf 'ls | grep foo && echo found || echo not found\n' | bash;       echo "bash=$?"
1.run these tests
2. fix each one of them to match the bash behaviour
3.recheck the tests
4.git after each fix