#define do1          1193180/33
#define do1_         1193180/35
#define re1          1193180/37
#define re1_         1193180/39
#define mi1          1193180/41
#define fa1          1193180/44
#define fa1_         1193180/46
#define so1          1193180/49
#define so1_         1193180/52
#define la1          1193180/55
#define la1_         1193180/58
#define si1          1193180/62
#define do2          1193180/65
#define do2_         1193180/69
#define re2          1193180/73
#define re2_         1193180/78
#define mi2          1193180/82
#define fa2          1193180/87
#define fa2_         1193180/93
#define so2          1193180/98
#define so2_         1193180/104
#define la2          1193180/110
#define la2_         1193180/117
#define si2          1193180/123
#define do3          1193180/131
#define do3_         1193180/139
#define re3          1193180/147
#define re3_         1193180/156
#define mi3          1193180/165
#define fa3          1193180/175
#define fa3_         1193180/185
#define so3          1193180/196
#define so3_         1193180/208
#define la3          1193180/220
#define la3_         1193180/233
#define si3          1193180/247
#define do4          1193180/262
#define do4_         1193180/277
#define re4          1193180/294
#define re4_         1193180/311
#define mi4          1193180/330
#define fa4          1193180/349
#define fa4_         1193180/370
#define so4          1193180/392
#define so4_         1193180/415
#define la4          1193180/440
#define la4_         1193180/466
#define si4          1193180/494
#define do5          1193180/523
#define do5_         1193180/554
#define re5          1193180/587
#define re5_         1193180/622
#define mi5          1193180/659
#define fa5          1193180/698
#define fa5_         1193180/740
#define so5          1193180/784
#define so5_         1193180/830
#define la5          1193180/880
#define la5_         1193180/932
#define si5          1193180/988
#define do6          1193180/1047
#define do6_         1193180/1109
#define re6          1193180/1175
#define re6_         1193180/1245
#define mi6          1193180/1319
#define fa6          1193180/1397
#define fa6_         1193180/1480
#define so6          1193180/1568
#define so6_         1193180/1661
#define la6          1193180/1760
#define la6_         1193180/1865
#define si6          1193180/1976
#define do7          1193180/2093
#define do7_         1193180/2217
#define re7          1193180/2349
#define re7_         1193180/2489
#define mi7          1193180/2637
#define fa7          1193180/2794
#define fa7_         1193180/2960
#define so7          1193180/3136
#define so7_         1193180/3322
#define la7          1193180/3520
#define la7_         1193180/3729
#define si7          1193180/3951
#define do8          1193180/4186

/* 자주 사용되는 음계의 영역을 간단하게 쓸 수 있도록 재 정의  */

#define _do          do3
#define _do_         do3_
#define _re          re3
#define _re_         re3_
#define _mi          mi3
#define _fa          fa3
#define _fa_         fa3_
#define _so          so3
#define _so_         so3_
#define _la          la3
#define _la_         la3_
#define _si          si3
#define _DO          do4
#define _DO_         do4_
#define _RE          re4
#define _RE_         re4_
#define _MI          mi4
#define _FA          fa4
#define _FA_         fa4_
#define _SO          so4
#define _SO_         so4_
#define _LA          la4
#define _LA_         la4_
#define _SI          si4

/* sound2.c -- simple melody */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <time.h>


main(argc, argv)
int argc;
char *argv[];
{
	int fd;
    fd=open("/dev/console", O_WRONLY|O_NDELAY);
	ioctl(fd, KIOCSOUND, do6);
    usleep(5*100000);
    ioctl(fd, KIOCSOUND, mi6);
    usleep(5*100000);
	ioctl(fd, KIOCSOUND, so6);
    usleep(5*100000);
	ioctl(fd, KIOCSOUND, do7);
    usleep(5*100000);
	ioctl(fd, KIOCSOUND, 0);
    close(fd);
}
