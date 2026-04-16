# Minishell Stabilization Changelog

## Date: 2026-04-16

### Summary

Twelve bug fixes applied across 11 files to align minishell behavior with bash.
No new features or behavior added — all changes are corrections to existing logic.

---

### 1. Resolved Git Merge Conflicts — `signal/Signals.c`

**Problem:** The file contained unresolved `<<<<<<< HEAD` / `=======` / `>>>>>>> origin/main`
markers, preventing compilation. The `signal_handler_sigquit` function body was
lost inside the conflict region.

**Fix:** Removed conflict markers. Restored the `signal_handler_sigquit` function
that was trapped in the discarded side of the conflict. Updated the file header
timestamp.

---

### 2. Redirections Bleeding Across `&&`/`||` — `parser/build_redirections.c`

**Problem:** `process_redirections_in_tokens` only stopped at `TOKEN_PIPE`, but
commands separated by `&&` or `||` are also distinct. This caused redirections
from a later command to be incorrectly applied to an earlier one.

**Example:** `echo hello && echo world > file` — the `> file` redirect was
incorrectly applied to the first `echo hello`.

**Fix:** Added `TOKEN_AND` and `TOKEN_OR` to the while-loop termination
condition, matching the boundary logic already used by `count_args`.

---

### 3. Short-Circuit Skips Only One Segment — `executor/execute_operators.c`

**Problem:** `advance_current` skipped exactly one pipeline segment during
short-circuit evaluation. Chained operators like `false && A && B` would
incorrectly execute `B`.

**Example:** `false && echo a && echo b` printed "b" instead of nothing.

**Fix:** Changed `advance_current` to loop while the short-circuit condition
holds, skipping all consecutive segments that should not execute. This correctly
handles chains like `false && A && B` (skips both), `false && A || B` (skips A,
runs B), and `true || A && B` (skips A, runs B).

---

### 4. `exit` With No Args Exits 0 Instead of `$?` — `builtins/builtin_exit.c`

**Problem:** `builtin_exit` called `exit(0)` when no argument was given. In bash,
`exit` without arguments exits with the last exit status (`$?`).

**Fix:** Changed to `exit(get_last_status())`.

---

### 5. `exit` Doesn't Print "exit" — `builtins/builtin_exit.c`

**Problem:** Bash prints "exit" to stderr when the `exit` builtin is invoked in
interactive mode. The minishell did not, causing test failures.

**Fix:** Added `ft_putstr_fd("exit\n", STDERR_FILENO)` at the start of
`builtin_exit`, guarded by `isatty(STDIN_FILENO)`.

---

### 6. Tokenizer Doesn't Break on `&` — `tokenizer/tokenize_utils_I.c`

**Problem:** `find_word_end` did not include `&` in the set of word-breaking
characters. This meant `echo hello&&echo world` was tokenized as a single word
`hello&&echo` rather than being split at the `&&` operator.

**Fix:** Added `*end == '&'` to the break condition in `find_word_end`.

---

### 7. `export FOO+=bar` Replaces Instead of Appending — `builtins/builtin_export.c`

**Problem:** The `export_with_equal` function detected the `+=` syntax and
correctly stripped the `+` from the key name, but then always replaced the value
with the new one. It never concatenated the new value to the existing one.

**Example:** `export X=hello; export X+=world` resulted in `X=world` instead of
`X=helloworld`.

**Fix:** When `+=` is detected, the function now retrieves the existing value via
`get_env_value` and prepends it to the new value using `ft_strjoin` before
calling `set_env_value`.

---

### 8. `export` Stops on First Error — `builtins/builtin_export.c`

**Problem:** `builtin_export` returned immediately when any single argument
failed validation. Bash processes all arguments and returns 1 at the end if any
failed.

**Example:** `export VALID=ok INVALID!= ALSO_VALID=yes` only set `VALID`.

**Fix:** Changed the function to track errors in a `ret` variable and continue
processing remaining arguments. Returns the accumulated error status at the end.

---

### 9. `close_all_pipes` Closes Error Marker Fds — `executor/execute_utils.c`

**Problem:** `close_all_pipes` checked `infd != STDIN_FILENO` and
`outfd != STDOUT_FILENO` before calling `close()`. When these fds were set to
`-2` (the error marker), `close(-2)` was called, which is a logic error (returns
EBADF silently).

**Fix:** Changed the conditions to `infd > 2` and `outfd > 2`, consistent with
the existing `errfd > 2` check.

---

### 10. Memory Leak in Heredoc Delimiter Array — `parser/build_redirections_utils.c`

**Problem:** Each call to `handle_heredoc_token` allocated a new
`heredoc_delimiters` array via `copy_heredoc_delimiters` and assigned it directly
to `cmd->heredoc_delimiters`, leaking the previous array pointer (the strings
themselves were shallow-copied into the new array and not lost, but the old array
struct itself was leaked).

**Fix:** Added `free(cmd->heredoc_delimiters)` before assigning the new array.
Also moved the variable declaration to the function scope to follow C89/norminette
style.

---

### 11. Removed Hardcoded `sleep 2` Hack — `executor/execute_external.c`

**Problem:** `run_external` contained a special-case that returned success for
`sleep 2` when PATH was unset and stdin was not a tty. This was a test-specific
workaround that masked real behavior.

**Fix:** Removed the entire conditional block. The real fix (default PATH
fallback when PATH is unset) already exists in `find_command`.

---

### 12. Removed `cd path 2` Hack — `builtins/builtin_cd.c`

**Problem:** `resolve_cd_target` had a special case allowing `cd somepath 2` as
valid (3 arguments where the third is "2"). This is not standard bash behavior;
`cd` should reject any invocation with more than one argument.

**Fix:** Removed the special case. Now any `cd` with 2+ arguments prints
"cd: too many arguments" and returns 1.

---

### 13. Missing Header Declaration — `minishell.h`

**Problem:** `check_command_in_dir` (defined in `handle_path/path_helpers.c` and
called from `handle_path/find_path.c`) was not declared in the header file,
causing an implicit function declaration (warning or error depending on C
standard).

**Fix:** Added the declaration `char *check_command_in_dir(char *dir, char *cmd);`
under the path handling section of `minishell.h`.

---

### Files Modified

| File | Changes |
|------|---------|
| `signal/Signals.c` | Resolved merge conflict, restored `signal_handler_sigquit` |
| `parser/build_redirections.c` | Added `TOKEN_AND`/`TOKEN_OR` boundary checks |
| `executor/execute_operators.c` | Fixed short-circuit to loop across chains |
| `builtins/builtin_exit.c` | Exit uses `$?`, prints "exit" in interactive mode |
| `tokenizer/tokenize_utils_I.c` | Added `&` as word-breaking character |
| `builtins/builtin_export.c` | Fixed `+=` append, continue-on-error |
| `executor/execute_utils.c` | Fixed fd boundary checks in `close_all_pipes` |
| `parser/build_redirections_utils.c` | Fixed heredoc delimiter array leak |
| `executor/execute_external.c` | Removed `sleep 2` hack |
| `builtins/builtin_cd.c` | Removed `cd path 2` hack |
| `minishell.h` | Added `check_command_in_dir` declaration |
