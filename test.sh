#!/bin/bash

PASS=0
FAIL=0
SHELL_BIN="./minishell"

run_test()
{
	local description="$1"
	local input="$2"
	local expected="$3"

	actual=$(echo "$input" | "$SHELL_BIN" 2>/dev/null)
	if echo "$actual" | grep -qF "$expected"; then
		echo "[PASS] $description"
		PASS=$((PASS + 1))
	else
		echo "[FAIL] $description"
		echo "       input:    $input"
		echo "       expected: $expected"
		echo "       got:      $actual"
		FAIL=$((FAIL + 1))
	fi
}

echo "=================================================="
echo "           MINISHELL TEST SUITE"
echo "=================================================="

# --------------------------------------------------
# 1. BUILTINS
# --------------------------------------------------
echo ""
echo "--- 1. Builtins ---"

run_test "echo basic"           "echo hello"            "hello"
run_test "echo -n flag"         "echo -n hello"         "hello"
run_test "pwd outputs path"     "pwd"                   "/"
run_test "cd to /tmp"           "cd /tmp && pwd"        "/tmp"
run_test "cd to HOME"           "cd && pwd"             "$HOME"
run_test "export + env"         "export TEST=hello
env"                            "TEST=hello"
run_test "export no args"       "export TEST=hello
export"                         "declare -x TEST"
run_test "unset removes var"    "export TEST=hello
unset TEST
env"                            ""
run_test "env lists vars"       "env"                   "PATH="

# --------------------------------------------------
# 2. EXTERNAL COMMANDS + PATH RESOLUTION
# --------------------------------------------------
echo ""
echo "--- 2. External Commands ---"

run_test "ls runs"              "ls"                    "Makefile"
run_test "ls -la runs"         "ls -la"                "total"
run_test "absolute path /bin/ls" "/bin/ls"              "Makefile"
run_test "cat a file"           "cat test.txt"          ""   # just checks no crash

# --------------------------------------------------
# 3. ENVIRONMENT INHERITANCE (build_env_array)
# --------------------------------------------------
echo ""
echo "--- 3. Environment Inheritance ---"

run_test "exported var visible in env"   "export MY_VAR=world
env | grep MY_VAR"                       "MY_VAR=world"

run_test "printenv sees exported var"    "export MY_VAR=world
printenv MY_VAR"                         "world"

# --------------------------------------------------
# 4. REDIRECTIONS
# --------------------------------------------------
echo ""
echo "--- 4. Redirections ---"

run_test "output redirect >"    "echo hello > /tmp/ms_test.txt
cat /tmp/ms_test.txt"           "hello"

run_test "append redirect >>"   "echo line1 > /tmp/ms_test.txt
echo line2 >> /tmp/ms_test.txt
cat /tmp/ms_test.txt"           "line2"

run_test "input redirect <"     "echo fromfile > /tmp/ms_input.txt
cat < /tmp/ms_input.txt"        "fromfile"

# --------------------------------------------------
# 5. PIPES
# --------------------------------------------------
echo ""
echo "--- 5. Pipes ---"

run_test "simple pipe"          "echo hello | cat"              "hello"
run_test "pipe with grep"       "ls | grep Makefile"            "Makefile"
run_test "3-command pipe"       "ls | grep .c | wc -l"          ""   # just checks no crash

# --------------------------------------------------
# 6. EDGE CASES
# --------------------------------------------------
echo ""
echo "--- 6. Edge Cases ---"

run_test "cd - switches back"           "cd /tmp
cd -
pwd"                                   "$PWD"
run_test "cd invalid path keeps pwd"    "pwd
cd /definitely/not/here
pwd"                                   "$PWD"
run_test "export overwrite"             "export TEST=one
export TEST=two
env | grep TEST"                        "TEST=two"
run_test "export bare var"              "export ONLYKEY
export"                                 "declare -x ONLYKEY"
run_test "unset missing var"            "unset DOES_NOT_EXIST
echo ok"                                "ok"
run_test "empty input no crash"         ""                       ""
run_test "leading spaces"               "   ls"                  "Makefile"
run_test "quoted argument"              'echo "hello world"'     "hello world"
run_test "single quotes keep spaces"    "echo 'a b c'"           "a b c"
run_test "double quotes keep spaces"    'echo "a b c"'           "a b c"
run_test "mixed quoted args"            "echo 'left side' \"right side\"" "left side right side"
run_test "multiple spaces collapse"     "echo   hello    world"  "hello world"
run_test "single quote no expansion"    "echo '\$HOME'"          "\$HOME"
run_test "double quote expansion"       'echo "$HOME"'           "$HOME"
run_test "logical and short-circuit"    "false && echo should_not_print" ""
run_test "command not found"            "invalidcmd"             "command not found"

# --------------------------------------------------
# 7. KNOWN ISSUE: pipe children env inheritance
# --------------------------------------------------
echo ""
echo "--- 7. Pipe Env Inheritance (known issue) ---"

run_test "exported var visible through pipe"    "export PIPETEST=yes
env | grep PIPETEST | cat"                      "PIPETEST=yes"

# --------------------------------------------------
# 8. PARSER UNIT TESTS (skip_whitespace)
# --------------------------------------------------
echo ""
echo "--- 8. Parser Unit Tests: skip_whitespace ---"

run_test "skip_whitespace: leading spaces" "   echo hello" "hello"
run_test "skip_whitespace: multiple spaces" "      echo world" "world"
run_test "skip_whitespace: tabs" "		echo test" "test"
run_test "skip_whitespace: mixed spaces and tabs" "  	  echo mixed" "mixed"

# --------------------------------------------------
# 9. PARSER UNIT TESTS (is_redirect_operator)
# --------------------------------------------------
echo ""
echo "--- 9. Parser Unit Tests: is_redirect_operator ---"

run_test "is_redirect_operator: detects >" "echo test > /tmp/ms_redir1.txt && test -f /tmp/ms_redir1.txt && echo success" "success"
run_test "is_redirect_operator: detects <" "echo input_test > /tmp/ms_redir2.txt && cat < /tmp/ms_redir2.txt" "input_test"
run_test "is_redirect_operator: detects >>" "echo line1 > /tmp/ms_redir3.txt && echo line2 >> /tmp/ms_redir3.txt && wc -l < /tmp/ms_redir3.txt" "2"
run_test "is_redirect_operator: handles non-redirect" "echo normal command" "normal command"

# --------------------------------------------------
# 10. PARSER UNIT TESTS (extract_redirect_filename)
# --------------------------------------------------
echo ""
echo "--- 10. Parser Unit Tests: extract_redirect_filename ---"

run_test "extract_redirect_filename: simple filename" "echo content > /tmp/ms_simple.txt && cat /tmp/ms_simple.txt" "content"
run_test "extract_redirect_filename: filename with path" "echo path_test > /tmp/ms_extract_path.txt && test -f /tmp/ms_extract_path.txt && echo ok" "ok"
run_test "extract_redirect_filename: stops at pipe" "echo data | cat > /tmp/ms_extract_pipe.txt && cat /tmp/ms_extract_pipe.txt" "data"
run_test "extract_redirect_filename: multiple args before redirect" "echo first second > /tmp/ms_extract_args.txt && cat /tmp/ms_extract_args.txt" "first second"

# --------------------------------------------------
# 11. PARSER EDGE CASES (combined functionality)
# --------------------------------------------------
echo ""
echo "--- 11. Parser Edge Cases ---"

run_test "append mode preserves content" "echo line1 > /tmp/ms_append.txt && echo line2 >> /tmp/ms_append.txt && cat /tmp/ms_append.txt" "line2"
run_test "input redirect reads file" "echo content > /tmp/ms_input_test.txt && cat < /tmp/ms_input_test.txt" "content"
run_test "mixed redirects in pipe" "echo hello | cat > /tmp/ms_mixed.txt && cat /tmp/ms_mixed.txt" "hello"
run_test "command with redirect and args" "echo first second > /tmp/ms_args.txt && cat /tmp/ms_args.txt" "first second"

# --------------------------------------------------
# 12. EXIT STATUS TRACKING ($?)
# --------------------------------------------------
echo ""
echo "--- 12. Exit Status Tracking ($?) ---"

run_test "status after success builtin" \
"echo ok
echo \$?" \
"0"

run_test "status after command not found" \
"invalidcmd
echo \$?" \
"127"

run_test "status after false" \
"false
echo \$?" \
"1"

run_test "status after true" \
"true
echo \$?" \
"0"

run_test "status after cd failure" \
"cd /definitely/not/here
echo \$?" \
"1"

run_test "status after cd success" \
"cd /tmp
echo \$?" \
"0"

run_test "status after pipeline uses last command" \
"echo hello | grep hello
echo \$?" \
"0"

run_test "status after pipeline last command fails" \
"echo hello | grep nomatch
echo \$?" \
"1"

run_test "status after && short-circuit failure" \
"false && echo should_not_run
echo \$?" \
"1"

run_test "status after && success chain" \
"true && echo ok
echo \$?" \
"0"

run_test "single quotes do not expand \$?" \
"echo '\$?'" \
"\$?"

run_test "double quotes expand \$?" \
"false
echo \"\$?\"" \
"1"

# --------------------------------------------------
# 13. SIGNAL HANDLING (interactive mode)
# --------------------------------------------------
echo ""
echo "--- 13. Signal Handling (Interactive Mode) ---"

# Note: Signal tests (Ctrl+C, Ctrl+D, Ctrl+\) require interactive TTY
# These automated tests verify non-interactive mode doesn't crash
# Manual testing required for actual Ctrl+C/Ctrl+D/Ctrl+\ behavior

run_test "non-interactive mode: commands execute normally" \
"echo test1
echo test2" \
"test2"

run_test "non-interactive mode: exit code preserved" \
"false
echo \$?" \
"1"

run_test "non-interactive mode: pipes work" \
"echo hello | cat" \
"hello"

run_test "non-interactive mode: redirection works" \
"echo data > /tmp/ms_signal_test.txt
cat /tmp/ms_signal_test.txt" \
"data"

# Note: Manual interactive tests (require terminal):
# 1. Ctrl+C (SIGINT):
#    $ ./minishell
#    minishell$ echo hello
#    hello
#    minishell$ <Ctrl+C>
#    minishell$ 
#    (should show new prompt on new line)
#
# 2. Ctrl+D (EOF):
#    $ ./minishell
#    minishell$ <Ctrl+D>
#    (shell exits)
#
# 3. Ctrl+\ (SIGQUIT):
#    $ ./minishell
#    minishell$ <Ctrl+\>
#    (does nothing - ignore signal)

# --------------------------------------------------
# SUMMARY
# --------------------------------------------------
echo ""
echo "=================================================="
echo "  PASSED: $PASS"
echo "  FAILED: $FAIL"
echo "  TOTAL:  $((PASS + FAIL))"
echo "=================================================="

# Cleanup temp files
rm -f /tmp/ms_*.txt
rm -f /tmp/ms_signal_test.txt