
#define BELL            putchar('\007')
#define GOTO_XY(x,y)    printf("\033[%d;%dH",y,x)
#define SET_AREA(x,y)   printf("\033[%d;%dr",x,y)
#define RESET_AREA      printf("\033[1;24r")

#define UMOVE(x)        printf("\033[%dA",x)
#define DMOVE(x)        printf("\033[%dB",x)
#define RMOVE(x)        printf("\033[%dC",x)
#define LMOVE(x)        printf("\033[%dD",x)

#define SCROLL_UP       printf("\033D")
#define SCROLL_DOWN     printf("\033M")

#define CLR_SCR_ALL     printf("\033[2J")
#define CLR_SCR(x)      printf("\033[%cJ",x)
#define DEL_LINE(x)     printf("\033[%cK",x)
#define ATTR_RESET      printf("\033[m")
#define ADD_BOLD        printf("\033[1m")
#define ADD_UNDER       printf("\033[4m")
#define ADD_BLINK       printf("\033[5m")
#define ADD_REVER       printf("\033[7m")
#define ADD_CONC        printf("\033[8m")
