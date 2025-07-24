#include <kernel/types.h>
#include <kernel/param.h>
#include <kernel/fcntl.h>
#include <user/user.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs <command> [args...].\n");
        exit(1);
    }

    char buf[512] = {'\0'};
    char *args[MAXARG] = {0};
    int arg_num = 0;
    for (; arg_num < argc - 1; arg_num++)
        args[arg_num] = argv[arg_num + 1];

    // it's impossible to have no pipe input
    // if (read(0, buf, sizeof(buf)) == 0) // no pipe input
    // {
    //     printf("%s\n", buf);
    //     int ret = fork();
    //     if (ret > 0)
    //     {
    //         int status;
    //         wait(&status);
    //         exit(0);
    //     }
    //     else
    //     {
    //         exec(args[0], args);
    //     }
    // }

    int total = 0;
    int n = 0;
    // caution: read in loop!!!
    while ((n = read(0, buf + total, sizeof(buf) - total - 1)) > 0)
    {
        total += n;
        if (total >= sizeof(buf) - 2)
            break;
    }

    int last_p = 0;
    int cur_p = 0;
    while (1)
    {
        if (cur_p >= total)
            break;
        while (buf[cur_p] != '\n')
            cur_p++;
        // have found a new arg
        char this_arg[128] = {'0'};
        memmove(this_arg, buf + last_p, cur_p - last_p);
        this_arg[cur_p - last_p] = '\0';
        cur_p++;
        last_p = cur_p;

        int ret = fork();
        if (ret > 0)
        {
            int status;
            wait(&status);
        }
        else
        {
            args[arg_num] = this_arg;
            exec(args[0], args);
        }
    }

    return 0;
}