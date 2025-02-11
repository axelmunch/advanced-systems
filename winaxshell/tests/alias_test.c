#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/hooks.h>
#include <string.h>
#include "alias.h"
#include "custom_commands.h"

void alias_setup(void)
{
    cr_redirect_stderr();
    cr_redirect_stdout();
}

TestSuite(alias, .init = alias_setup);

Test(alias, test_free_alias)
{
    set_alias("a", "toto");
    set_alias("b", "titi");

    cr_assert_eq(get_alias_count(), 2, "There should be 2 aliases, got %d", get_alias_count());

    free_aliases();
    cr_assert_eq(get_alias_count(), 0, "There should be 0 aliases after freeing, got %d", get_alias_count());
}

Test(alias, test_get_alias_name)
{
    set_alias("a", "toto");
    set_alias("b", "titi");

    int status = get_alias_name_by_index(-1, "test");
    cr_assert_eq(status, -1, "Should return -1 for invalid index, got %d", status);

    free_aliases();
}

Test(alias, test_get_alias_command)
{
    set_alias("a", "toto");
    set_alias("b", "titi");

    char *nonexistent = get_alias_command("nonexistent");
    cr_assert_null(nonexistent, "Should return NULL for nonexistent alias");
}

Test(alias, test_set_alias_overflow)
{
    for (int i = 0; i < MAX_ALIAS; i++)
    {
        char alias[10];
        char command[10];
        sprintf(alias, "a%d", i);
        sprintf(command, "t%d", i);
        set_alias(alias, command);
    }

    char *command_final = "alias";
    char *args_final[] = {"alias", "a", NULL};

    errno = EXIT_SUCCESS;

    bool result = execute_custom_command_main_process(command_final, args_final);
    cr_assert_eq(result, false, "Should return false when trying to set too many aliases");

    free_aliases();
}
