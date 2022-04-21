#include <stdio.h>
main()
{
    chdir("/bbs");
    execl("bin/main","main",NULL);
}
