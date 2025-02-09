#include "helper.h"

/**
 * @brief Internal binary options declaration (must end with {0,0,0,0})
 * @param help
 * @param verbose
 * @param command
 * @return struct option
 */
static struct option binary_opts[] = {
    {"help", no_argument, 0, 'h'},
    {"verbose", no_argument, 0, 'v'},
    {"command", required_argument, 0, 'c'},
    {0, 0, 0, 0}};

static char *command_requested = NULL;

char *get_requested_command()
{
    return command_requested;
}

char *dup_optarg_str()
{
    char *str = NULL;
    if (optarg != NULL)
    {
        str = strndup(optarg, MAX_INPUT_LENGTH);
        if (str == NULL)
        {
            print_error("[ERROR] strndup() failed");
        }
    }
    return str;
}

void show_help(char **argv)
{
    print_generic(STDOUT_FILENO, "USAGE: %s %s\n\n%s\n", argv[0], USAGE_SYNTAX, USAGE_PARAMS);
}

void show_parameters(bool verbose_mode, bool command_mode)
{
    print("** PARAMS **\n%-8s: %d\n%-8s: %d\n",
          "verbose", verbose_mode,
          "command", command_mode);
}

void check_requirements(int argc, char **argv)
{
    return;
}

void parse_options(int argc, char **argv, bool *command_mode, bool *no_execute)
{
    *no_execute = false;
    *command_mode = false;

    int opt = -1;
    int opt_idx = -1;

    while ((opt = getopt_long(argc, argv, BINARY_OPTION_STR, binary_opts, &opt_idx)) != -1)
    {
        switch (opt)
        {
        case 'c':
            // Command param
            if (optarg)
            {
                command_requested = dup_optarg_str();

                if (command_requested == NULL)
                {
                    errno = EINVAL;
                    print_error("[ERROR] Command is NULL");
                }

                if (!*no_execute)
                {
                    *command_mode = true;
                }
            }
            break;
        case 'v':
            set_verbose_mode(true);
            break;
        case 'h':
            show_help(argv);
            *no_execute = true;
            *command_mode = false;
            break;
        default:
            *no_execute = true;
            break;
        }
    }
}

void print_prompt()
{
    char cwd[MAX_INPUT_LENGTH];
    char *username = getenv("USER");

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        print_generic(STDOUT_FILENO, GREEN_COLOR "%s@%s" RESET_COLOR ":" BLUE_COLOR "%s\n" RESET_COLOR "$ ", username, SHELL_NAME, cwd);
    }
    else
    {
        print_error("getcwd() error");
        exit(EXIT_FAILURE);
    }
}

void predator_shake(void)
{
    static const char *art_lines[] = {
        "    .................      . .:.:-.# ...........................................",
        "      ...............  @*%#+:-:=#@@* .........................................  ",
        "  .    ...............  @@@#+@@@%**@@  .....................................    ",
        ".@@%=    ............. .@%=*#@@@@@@#@  ............................           @#",
        " =*%@@@@    .......... @@%@@%@+=*-%@@@  ........................     -@@@@@@@###",
        " ##%#+:#@@    ........ *@@%%@@**@+=*@@. ....................    +@#@@%###%%%%@%*",
        " ++-::+@++@@:  ....... . @%++@@@@@**%@@  .................  .-##%############%%*",
        " +=-=*@..:-@@   .......  :@@+**#%%+*%@@. ...............   :###%##%########%#%%*",
        ".++=+*=-**@@@@-  .......  .@%*%%@%*%@@@. ............   :%###@##%####%##%####%%*",
        " +++*###=-+.=-*   .......  @@@-%%+*#@@%   ......      .%##%@%%@%########%#####%*",
        " =+#+=+=:=*@@-%@ .  .....  .@. @#*%@%@#    ...    @@@@@@########%@#%########%#%*",
        " #@#+*+=+@@%..:@@@    ....  @ #@**###@@@@    . =-@@#****=:+###%##@##%##########*",
        " %%###-#@%=  #@@**@@#    .  .@%#***@#@*#@@@    .@%**+:.=-=-*##%@%@##%%%%##%####*",
        " @%+=++@*  .@@*:.=:=+@@   .@@@@@%#*##@+**@@@-  @@#+-:+####*+%##%%@%#%%%%%%%%%%%*",
        ".%**++@=..=@#.:.===- .@@ . @%#*######@**#=+@@@@%#%@%%%%##%@@@%###@@#%%%%%%%%%%%*",
        ".%@*+#*- *@#-::===-..@@@@@@@%#*#+*##%@**#*#@+#*==-#@@@@@@@+..%#%@@@##%%%%%%%%%%*",
        ".%%@##  @@#=:====:..@@@%*@@+***#%%%#@%##%**##*+@@%*:.-++*+++=*#%@@@##%%%#%#%%%%*",
        "-@#-:+@@@#:--#+-. @@%%%@#=%%*+*@@%#@@###*#%***+*#@-*@@*#=:+*#@@@@@@@%%%%%%%%%%%*",
        "%@@@@@@+=+#@*-. @@@@%%##@@@*==*@@#@@@#%%%##*###%=-.-=%%*+*%@@@@@   .@%%%%%%%%%%*",
        ":@@@@@@@@#*:.=@@@@%%%%%##%#%%#@@%@@@%#%%%%%%@@#=::+%%%%+@@@@@-     .@%%%%%%%%%%*",
        "+@@@@@@@@-@@@@@@@@@@%%%%###%#@@@@@@@@@@@%%@@@*=+#%@@@@@@@@@    ...  @%@%%%%%%%%*",
        "+@@%%@%@@ .@@@@@@@@@@@@@@@@@#%@@@@@  @@%%%%*+#@@@@@@@@%      .....  @@@@%%%%%%%*",
        "-:..++=%     -##**#####*****#**+=+%   *+++++**-             ......  =#**********",
    };

    size_t num_lines = sizeof(art_lines) / sizeof(art_lines[0]);
    for (size_t i = 0; i < num_lines; i++)
        print_generic(STDOUT_FILENO, GRAY_COLOR "%s\n" RESET_COLOR, art_lines[i]);
}