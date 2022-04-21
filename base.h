#ifndef XHOST_BASE
#define XHOST_BASE

#define fseektop(a)   rewind((a))
#define fseekend(a)   fseek((a),0,SEEK_END)
#define byte   unsigned char
#define word   unsigned int
#define No 0
#define Yes 1

#define Nostop 0
#define NoStop 0
#define Stop   1

char tty[20]; // 접속 포트
char htty[20];// 호스트번호별포트

#include "ansi.h"

#endif	/* XHOST_H */
