/* terminal mode */
#define VT100 "[61\"p"
#define VT2008 "[62\"p"
#define VT2007 "[62;1\"p"
/* 7 bit , 8 bit control */
#define BIT7 " F"
#define BIT8 " G"
/* keyboard lock */
#define KEY_LOCK "[2h"
#define KEY_UNLOCK "[2l"
/* insert mode , replace */
#define INSERT_MODE "[4h"
#define REPLACE_MODE "[4l"
/* cursor set  */
#define CURSOR_ON "[?25h"
#define CURSOR_OFF "[?25l"
/* cursor key mode set  */
#define APPL_CURSOR "[?1h"
#define NORMAL_CURSOR "[?1l"
/* screen width  set  */
#define COL_132 "[?3h"
#define COL_80 "[?3l"
/* screen attribute  set  */
#define REVERSE_ON "[?5h"
#define REVERSE_OFF "[?5l"
/* screen scroll  set  */
#define SMOOTH_SCROLL "[?4h"
#define JUMP_SCROLL "[?4l"
/* origin mode  set  */
#define RELATIVE "[?6h"
#define ABSOLUTE "[?6l"
/* auto wrap mode  set  */
#define AUTO_WRAP "[?7h"
#define NO_AUTO_WRAP "[?7l"
/* hangeul,english mode  set  */
#define HAN "[?85h"
#define ENGLISH "[?85l"
/* hangeul  character set  */
#define KS_5601 "$*7"   /* fast terminal */
/* clear screen */
#define CLEAR_SCREEN "[2J"   
#define CLEAR_ATTR "[0\"q"   

