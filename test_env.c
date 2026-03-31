
#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
    struct s_env *prev;
} t_env;

void print_env(t_env *env) {
    while (env) {
        printf("%s=%s\n", env->key, env->value ? env->value : "");
        env = env->next;
    }
}

int main() {
    printf("--- Testing builtin_env ---\n");
    // Create mock environment list
    t_env env3 = {"PATH", "/usr/bin", NULL, NULL};
    t_env env2 = {"HOME", "/home/user", &env3, NULL};
    t_env env1 = {"USER", "test", &env2, NULL};
    env2.prev = &env1;
    env3.prev = &env2;

    // Test: correct usage
    char *env_args1[] = {"env", NULL};
    builtin_env(env_args1, &env1); // Should print all key=value pairs

    // Test: too many arguments
    char *env_args2[] = {"env", "extra", NULL};
    builtin_env(env_args2, &env1); // Should print error to stderr

    printf("\n--- Testing builtin_cd ---\n");
    t_env cd_env1 = {"HOME", "/tmp", NULL, NULL}; // Set HOME to /tmp
    t_env *cd_env = &cd_env1;
    char cwd[4096];

    // Test: cd with no arguments (should go to HOME)
    char *cd_args1[] = {"cd", NULL};
    builtin_cd(cd_args1, &cd_env);
    getcwd(cwd, sizeof(cwd));
    printf("Current directory after 'cd': %s\n", cwd);

    // Test: cd to a specific directory
    char *cd_args2[] = {"cd", "/usr", NULL};
    builtin_cd(cd_args2, &cd_env);
    getcwd(cwd, sizeof(cwd));
    printf("Current directory after 'cd /usr': %s\n", cwd);

    // Test: cd with too many arguments
    char *cd_args3[] = {"cd", "dir1", "dir2", NULL};
    builtin_cd(cd_args3, &cd_env);

    printf("\n--- Testing update_existing_env_value ---\n");
    t_env upd_env1 = {strdup("USER"), strdup("bob"), NULL, NULL};

    // Test 1: Update existing key
    int ret = update_existing_env_value(&upd_env1, "USER", "alice");
    printf("Test 1: %d, USER=%s\n", ret, upd_env1.value); // Should print 1, USER=alice

    // Test 2: Update non-existing key
    ret = update_existing_env_value(&upd_env1, "HOME", "/home/bob");
    printf("Test 2: %d\n", ret); // Should print 0

    // Test 3: NULL value for existing key
    ret = update_existing_env_value(&upd_env1, "USER", NULL);
    printf("Test 3: %d, USER=%s\n", ret, upd_env1.value); // Should print 1, USER=alice

    // Cleanup
    free(upd_env1.key);
    free(upd_env1.value);

    return 0;
}