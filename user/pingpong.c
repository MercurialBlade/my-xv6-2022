#include <kernel/types.h>
#include <user/user.h>

// // one pipe implementation
// int main(int argc, char *argv[])
// {
//     int p[2];
//     pipe(p);

//     int ret = fork();
//     if (ret == 0) // child
//     {
//         int pid = getpid();

//         char buf[] = "a";
//         read(p[0], buf, 1);
//         printf("%d: received ping\n", pid);
//         close(p[0]);
//         write(p[1], buf, 1);
//         close(p[1]);
//         exit(0);
//     }
//     else
//     {
//         int pid = getpid();

//         char buf[] = "a";
//         write(p[1], buf, 1);
//         close(p[1]);
//         read(p[0], buf, 1);
//         close(p[0]);
//         printf("%d: received pong\n", pid);

//         exit(0);
//     }
// }

// two pipes implementation
int main(int argc, char *argv[])
{
    int p1[2]; // parent->child
    int p2[2]; // child->parent
    pipe(p1);
    pipe(p2);

    int ret = fork();
    if (ret == 0) // child
    {
        int pid = getpid();

        char buf[] = "a";
        read(p1[0], buf, 1);
        printf("%d: received ping\n", pid);
        write(p2[1], buf, 1);
        close(p2[1]);
        close(p1[0]);
        exit(0);
    }
    else
    {
        int pid = getpid();

        char buf[] = "a";
        write(p1[1], buf, 1);
        read(p2[0], buf, 1);
        close(p1[1]);
        close(p2[0]);
        printf("%d: received pong\n", pid);

        exit(0);
    }
}