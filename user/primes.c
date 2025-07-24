#include <kernel/types.h>
#include <user/user.h>

#define SIZE_INT 4

void feed_nums(int p[], int start_num, int end_num)
{
    close(p[0]); // parents only write
    for (int i = start_num; i <= end_num; i++)
    {
        write(p[1], &i, SIZE_INT);
    }
    close(p[1]); // parents' task has been done
}

void sieve_primes(int p[])
{
    close(p[1]);

    int first_prime = 0;
    read(p[0], &first_prime, SIZE_INT);
    printf("prime %d\n", first_prime);

    int next_num = 0;
    if (read(p[0], &next_num, SIZE_INT) == 0)
    {
        return;
    }

    int p_new[2];
    pipe(p_new);
    // printf("%d process create pipe: %d->%d\n", getpid(), p_new[0], p_new[1]); // for debug

    int ret = fork();
    if (ret > 0)
    {
        close(p_new[0]);
        // using the do-while loop because the first next_num has been read above
        do
        {
            if (next_num % first_prime != 0)
            {
                write(p_new[1], &next_num, SIZE_INT);
            }
        } while (read(p[0], &next_num, SIZE_INT) > 0);
        close(p[0]);
        close(p_new[1]); // this child process has written all the suspects to the grandchild

        int status;
        wait(&status);

        exit(0);
    }
    else
    {
        close(p[0]);
        sieve_primes(p_new);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        printf("Fail: incorrect argc!\n");
        exit(1);
    }

    int p[2];
    pipe(p);

    int ret = fork();
    if (ret > 0)
    {
        feed_nums(p, 2, 35);

        int status;
        wait(&status);
        exit(0);
    }
    else
    {
        sieve_primes(p);

        int status;
        wait(&status);
        exit(0);
    }

    return 0;
}