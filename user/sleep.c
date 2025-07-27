#include <kernel/types.h>
#include <user/user.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Error! Usage: [sleep ticks].\n");
        // printf("exec sleep failed.");
        exit(1);
    }
    int ticks = atoi(argv[1]);
    sleep(ticks);
    exit(0);
}
