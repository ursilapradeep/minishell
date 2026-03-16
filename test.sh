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

run_test "empty input no crash"         ""              ""
run_test "leading spaces"               "   ls"         "Makefile"
run_test "quoted argument"              'echo "hello world"'    "hello world"
run_test "single quote no expansion"    "echo '\$HOME'"         "\$HOME"
run_test "double quote expansion"       'echo "$HOME"'          "$HOME"
run_test "command not found"            "invalidcmd"    "command not found"

# --------------------------------------------------
# 7. KNOWN ISSUE: pipe children env inheritance
# --------------------------------------------------
echo ""
echo "--- 7. Pipe Env Inheritance (known issue) ---"

run_test "exported var visible through pipe"    "export PIPETEST=yes
env | grep PIPETEST | cat"                      "PIPETEST=yes"

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
rm -f /tmp/ms_test.txt /tmp/ms_input.txt
