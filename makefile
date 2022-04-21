#SHELL=	/bin/sh
#CC=	 gcc -g -W
CC= 	gcc -W -O3
CFLAGS= -O3
LFLAGS=

LIBS=
LLIBS=

# what are we making
OBJS	=main.o util.o view.o xchat.o xchatut.o logo.o bbsread.o error.o ip.o exec.o mterm.o startid.o startlog.o xtest.o bbssub.o sub.o write.o gocode.o check.o mail.o maile.o smemo.o comm.o sys_menu.o bbs.o down.o mess.o
ED_MENU	=ed_menu.o util.o
GUEST	=guest.o guestut.o util.o
XCONV	=xconv.o util.o
ZBTOTAL	=zbtotal.o util.o
XFRM	=xfrm.o util.o
BDN	=bdn.o util.o
S_OBJ	=sysop.o sysops.o util.o
B_OBJ	=bsysop.o bsysoput.o ip.o util.o
PFINFO	=pfinfo.o util.o
TXTBBS	=txt2bbs.o util.o
ED_BBS	=ed_bbs.o util.o
VIEW_US =view_us.o util.o
VIEWLOG =viewlog.o util.o
XHELP 	=xhelp.o util.o
XLOGO 	=xlogo.o util.o
XMENU 	=xmenu.o util.o
PF	=pf.o util.o

# rules
all:	m
u:	biu
cpbbs:
	cp /bbs/bin/* /xbase/bin -f
rx:
	cp m /xbase/bin/main -f
	cp sysop /xbase/bin/ -f
	cp bsysop /xbase/bin/ -f
	cp csysop /xbase/bin/ -f
	strip /xbase/bin/*

m:	$(OBJS)
	./makenum;$(CC) $(LFLAGS) -o $@ $(OBJS) -lm $(LIBS)
	chmod 777 m 
pf:	$(PF)
	$(CC) -o $@ $(PF)
zbtotal:	$(ZBTOTAL)
	$(CC) -o $@ $(ZBTOTAL)
bdn:	$(BDN)
	$(CC) -o $@ $(BDN)
xfrm:	$(XFRM)
	$(CC) -o $@ $(XFRM)
XCHECK	=xcheck.o util.o
xcheck:	$(XCHECK)
	$(CC) -o $@ $(XCHECK)
GIVE	=give.o util.o
xconv:	$(XCONV)
	$(CC) -o $@ $(XCONV)
GIVE	=give.o util.o
give:	$(GIVE)
	$(CC) -o $@ $(GIVE)
GET_MYPF	=get_mypf.o util.o
get_mypf:	$(GET_MYPF)
	$(CC) -o $@ $(GET_MYPF)
H2X	=h2x.o util.o
h2x:	$(H2X)
	$(CC) -o $@ $(H2X)
KILLUS	=killus.o util.o
killus:	$(KILLUS)
	$(CC) -o $@ $(KILLUS)
KOKOMON	=kokomon.o util.o
kokomon:	$(KOKOMON)
	$(CC) -o $@ $(KOKOMON)
DELMAIL	=delmail.o util.o
delmail:	$(DELMAIL)
	$(CC) -o $@ $(DELMAIL)
xlogo:	$(XLOGO)
	$(CC) -o $@ $(XLOGO)
xmenu:	$(XMENU)
	$(CC) -o $@ $(XMENU)
xhelp:	$(XHELP)
	$(CC) -o $@ $(XHELP)
pfinfo: $(PFINFO)
	$(CC) -o $@ $(PFINFO)
BBSRAND	=bbsrand.o util.o
bbsrand:	$(BBSRAND)
	$(CC) -o $@ $(BBSRAND)
XCD	=xcd.o util.o
xcd:	$(XCD)
	$(CC) -o $@ $(XCD)
QUESTBBS=questbbs.o util.o
questbbs:	$(QUESTBBS)
	$(CC) -o $@ $(QUESTBBS)
QUESTPDS=questpds.o util.o
questpds:	$(QUESTPDS)
	$(CC) -o $@ $(QUESTPDS)
QUEST	=quest.o util.o
quest:	$(QUEST)
	$(CC) -o $@ $(QUEST)
DIR2BBS	=dir2bbs.o util.o
dir2bbs:	$(DIR2BBS)
	$(CC) -o $@ $(DIR2BBS)
XVOTE	=xvote.o util.o
xvote:	$(XVOTE)
	$(CC) -o $@ $(XVOTE)
SE_DOWN =se_down.o util.o
se_down: $(SE_DOWN)
	$(CC) -o $@ $(SE_DOWN)
ed_menu: $(ED_MENU)
	$(CC) $(LFLAGS) -o $@ $(ED_MENU) $(LIBS)

sysop: $(S_OBJ)
	$(CC) $(LFLAGS) -o $@ $(S_OBJ) $(LIBS)

bsysop: $(B_OBJ)
	$(CC) $(LFLAGS) -o $@ $(B_OBJ) $(LIBS)
FINDMENU	=findmenu.o util.o
findmenu: $(FINDMENU)
	$(CC) $(LFLAGS) -o $@ $(FINDMENU) $(LIBS)
ED_PF	=ed_pf.o util.o
ed_pf: $(ED_PF)
	$(CC) $(LFLAGS) -o $@ $(ED_PF) $(LIBS)
CGUEST	=cguest.o util.o
cguest: $(CGUEST)
	$(CC) $(LFLAGS) -o $@ $(CGUEST)
CSYSOP	=csysop.o util.o
csysop: $(CSYSOP)
	$(CC) $(LFLAGS) -o $@ $(CSYSOP)
guest: $(GUEST)
	$(CC) $(LFLAGS) -o $@ $(GUEST) $(LIBS)

txt2bbs: $(TXTBBS)
	$(CC) $(LFLAGS) -o $@ $(TXTBBS) $(LIBS)

view_us: $(VIEW_US)
	$(CC) -o $@ $(VIEW_US)

viewlog: $(VIEWLOG)
	$(CC) -o $@ $(VIEWLOG)

ed_bbs: $(ED_BBS)
	$(CC) -o $@ $(ED_BBS)

sm:
	cd smailsrc;touch *.c;make;cp smail .. 

s:
	strip m sysop csysop 
c:
	rm -f *.o core
r:
	cp m /bbs/bin/main -rf
	cp sysop /bbs/bin -rf
	cp bsysop /bbs/bin -rf
	cp csysop /bbs/bin -rf
	chmod 777 /bbs/bin/*

mail:
	cd smailsrc;touch *.c;make;cp smail ..
	cd xmailsrc;touch *.c;make xmail;cp xmail ..
	cd emailsrc;touch *.c;make installe;cp installe ..
	make del_mail
editor:
	cd edit_src/editor1/;touch *.c;make
	cd edit_src/editor2/;touch *.c;make editor2;
	cp edit_src/editor1/pico     /bbs/bin/editor1  -f
	cp edit_src/editor2/editor2  /bbs/bin/editor2  -f
bi:
	make makenum
	make guest
	make	&
	make sysop&
	make bsysop&
	make csysop&
	make h2x
	make xmessd
	make xmenu
	make xlockd
	make xcheck
	make start
	make viewlog
	make view_us
	make killus
	make kokomon
	make pf
	make sp2file
	make findmenu
	make check_sh
	make findbbs
	make ed_menu
	make ed_bbs
	make pfinfo
	make ed_pf
	make xfrm
	make ed_lev
	make get_mypf
	make give
	make xvote
	make svote
	make quest
	make questpds
	make questbbs
	make cguest
	make txt2bbs
	make bdn
	make xhelp
	make dir2bbs
	make set_warn
	make zbtotal
	make rfile
	make xlogo
	make xcd
	make xlower
	make bbsrand
	make cd2bbs
	make check_te
	make xwall
	make xbcal
	make delmail
	make se_down

biu:
	cp m /bbs/bin/main -rf
	cp view_us /bbs/bin -rf
	cp se_down /bbs/bin -rf
	cp pfinfo /bbs/bin -rf
	cp xmessd /bbs/bin -rf
	cp xlockd /bbs/bin -rf
	cp xcheck /bbs/bin -rf
	cp sysop /bbs/bin -rf
	cp guest /bbs/bin -rf
	cp check_sh /bbs/bin -rf
	cp start /bbs/bin -rf
	cp xmenu /bbs/bin -rf
	cp rfile /bbs/bin -rf
	cp bsysop /bbs/bin -rf
	cp csysop /bbs/bin -rf
	cp viewlog /bbs/bin -f
	cp kokomon /bbs/bin -rf
	cp findmenu /bbs/bin -rf
	cp findbbs /bbs/bin -rf
	cp killus /bbs/bin -rf
	cp sp2file /bbs/bin -rf
	cp h2x /bbs/bin -rf
	cp ed_menu /bbs/bin -rf
	cp ed_bbs /bbs/bin -rf
	cp delmail /bbs/bin -rf
	cp ed_pf /bbs/bin -rf
	cp xfrm /bbs/bin -rf
	cp ed_lev /bbs/bin -rf
	cp get_mypf /bbs/bin -rf
	cp give /bbs/bin -rf
	cp xvote /bbs/bin -rf
	cp svote /bbs/bin -rf
	cp quest /bbs/bin -rf
	cp guest /bbs/bin -rf
	cp cguest /bbs/bin -rf
	cp txt2bbs /bbs/bin -rf
	cp bdn /bbs/bin -rf
	cp xhelp /bbs/bin -rf
	cp dir2bbs /bbs/bin -rf
	cp set_warn /bbs/bin -rf
	cp zbtotal /bbs/bin -rf
	cp questbbs /bbs/bin -rf
	cp questpds /bbs/bin -rf
	cp rfile /bbs/bin -rf
	cp pf /bbs/bin -rf
	cp xlogo /bbs/bin -rf
	cp xcd /bbs/bin -f
	cp xlower /bbs/bin -f
	cp bbsrand /bbs/bin -f
	cp cd2bbs /bbs/bin -f
	cp check_te /bbs/bin -f
	cp xwall /bbs/bin -f
	cp xbcal /bbs/bin -f
	strip /bbs/bin/*
	rm /bbs/bin/sta0* -f
	chmod 777 /bbs/bin
