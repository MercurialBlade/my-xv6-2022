#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "user/user.h"

char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // printf("Here2\n");
    // printf("%d\n", strlen(p));
    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
    {
        return p;
    }
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void find_function(char *cur_path, char *target)
{
    // copy from ls.c
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(cur_path, O_RDONLY)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", cur_path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", cur_path);
        close(fd);
        return;
    }
    // copy from ls.c

    switch (st.type)
    {
    case T_FILE:
    {
        char *file_name = fmtname(cur_path);
        // printf("%s\n", file_name);
        // printf("%s\n", target);

        // caution: In ls.c, the return value of fmtname will be blank-padded.
        char *temp = target;
        temp[DIRSIZ] = 0;
        memset(temp + strlen(temp), ' ', DIRSIZ - strlen(temp));

        int is_match = 1;
        if (strcmp(file_name, temp) != 0)
            is_match = 0;
        if (is_match)
            printf("%s\n", cur_path);
        close(fd);
        break;
    }

    case T_DIR:
        strcpy(buf, cur_path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            else if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            // printf("Here1\n");
            // printf("%d\n", strlen(p));
            find_function(buf, target);
        }
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "Usage: find <directory> <target filename>.\n");
        exit(1);
    }
    else
    {
        find_function(argv[1], argv[2]);
    }
    return 0;
}