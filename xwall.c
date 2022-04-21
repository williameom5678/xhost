
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/uio.h>

#include <paths.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utmp.h>
#ifdef __linux__
#include <locale.h>

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	@(#)pathnames.h	5.3 (Berkeley) 5/9/89
 *
 * Changed: Sun Nov 21 12:30:54 1993 by faith@cs.unc.edu
 * Changed: Wed Jun 22 20:47:27 1994 by faith@cs.unc.edu, based on changes
 *                                   from poe@daimi.aau.dk
 * Changed: Wed Jun 22 22:50:13 1994 by faith@cs.unc.edu
 * Changed: Sat Feb  4 16:02:10 1995 by faith@cs.unc.edu
 */

#ifndef __STDC__
# error "we need an ANSI compiler"
#endif

/* The paths for some of these are wrong in /usr/include/paths.h, but we
   re-define them here. */

#undef _PATH_UTMP
#undef _PATH_WTMP
#undef _PATH_DEFPATH
#undef _PATH_DEFPATH_ROOT
#undef _PATH_LASTLOG
#undef _PATH_MAILDIR

#ifndef SBINDIR
#define SBINDIR			"/sbin"
#endif

#ifndef USRSBINDIR
#define USRSBINDIR              "/usr/sbin"
#endif

#ifndef LOGDIR
#define LOGDIR                  "/var/log"
#endif

#ifndef VARPATH
#define VARPATH			"/var"
#endif

#define _PATH_BSHELL    	"/bin/sh"
#define _PATH_CSHELL    	"/bin/csh"
//#define UT_NAMESIZE         8
#define _PATH_TTY       	"/dev/tty"
#define TTYTYPES        	"/etc/ttytype"
#define SECURETTY       	"/etc/securetty"
#define _PATH_UTMP      	"/var/run/utmp"
#define _PATH_WTMP      	LOGDIR "/wtmp"

#define	_PATH_DEFPATH	        "/usr/local/bin:/bin:/usr/bin:."
#define	_PATH_DEFPATH_ROOT	SBINDIR ":/bin:" USRSBINDIR ":/usr/bin"
#define	_PATH_HUSHLOGIN		".hushlogin"
#define	_PATH_LASTLOG		LOGDIR "/lastlog"
#define	_PATH_MAILDIR		VARPATH "/spool/mail"
#define	_PATH_MOTDFILE		"/etc/motd"
#define	_PATH_NOLOGIN		"/etc/nologin"

#define _PATH_LOGIN		"/bin/login"
#define _PATH_INITTAB		"/etc/inittab"
#define _PATH_RC		"/etc/rc"
#define _PATH_REBOOT		SBINDIR "/reboot"
#define _PATH_SINGLE		"/etc/singleboot"
#define _PATH_SECURE		"/etc/securesingle"
#define _PATH_USERTTY           "/etc/usertty"

#define _PATH_MTAB		"/etc/mtab"
#define _PATH_UMOUNT		"/bin/umount"
#define UMOUNT_ARGS		"umount", "-a"
#define SWAPOFF_ARGS            "swapoff", "-a"

#define _PATH_PASSWD            "/etc/passwd"
#define _PATH_PTMP              "/etc/ptmp"
#define _PATH_PTMPTMP           "/etc/ptmptmp"

#define _PATH_WORDS             "/usr/dict/words"
#define _PATH_WORDS_ALT         "/usr/dict/web2"





#endif

#include <sys/types.h>
#include <sys/uio.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <paths.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void    makemsg __P((char *,char *));

#define	IGNOREUSER	"sleeper"

int mbufsize;
char *mbuf;

/* ARGSUSED */
int
main(argc, argv)
	int argc;
	char **argv;
{
	extern int optind;
	int ch;
	struct iovec iov;
	struct utmp utmp;
	FILE *fp;
	char *p, *ttymsg();
	char line[sizeof(utmp.ut_line) + 1];

	setlocale(LC_CTYPE,"");
    makemsg(argv[1],argv[2]);

    if (!(fp = fopen(_PATH_UTMP, "r+"))) {
        if (!(fp = fopen("/etc/utmp", "r+"))) {
            (void)fprintf(stderr, "wall: cannot read %s.\n", _PATH_UTMP);
            exit(1);
        }
	}
	iov.iov_base = mbuf;
	iov.iov_len = mbufsize;
	/* NOSTRICT */
	while (fread((char *)&utmp, sizeof(utmp), 1, fp) == 1) {
		if (!utmp.ut_name[0] ||
		    !strncmp(utmp.ut_name, IGNOREUSER, sizeof(utmp.ut_name)))
			continue;
#ifdef __linux__
		if (utmp.ut_type != USER_PROCESS)
		   continue;
#endif
        strncpy(line, utmp.ut_line, sizeof(utmp.ut_line));
		line[sizeof(utmp.ut_line)] = '\0';
	}
	exit(0);
}

void
makemsg(char *fname,char *tid)
{
	register int ch, cnt;
	struct tm *lt;
	struct passwd *pw;
	struct stat sbuf;
	time_t now, time();
	FILE *fp;
	int fd;
	char *p, *whom, hostname[MAXHOSTNAMELEN], lbuf[100], tmpname[15];
	char *getlogin(), *ttyname();
//	char *getlogin(), *strcpy(), *ttyname();

	(void)strcpy(tmpname, _PATH_TMP);
	(void)strcat(tmpname, "/wall.XXXXXX");
    if (!(fd = mkstemp(tmpname)) || !(fp = fdopen(fd, "r+"))) exit(1);
	(void)unlink(tmpname);

    (void)gethostname(hostname, sizeof(hostname));
    (void)time(&now);
    lt = localtime(&now);
    (void)fprintf(fp, "\r\n   ### %s님의 전체 메세지입니다. %d:%02d ...### \r\n",tid,lt->tm_hour, lt->tm_min);
    if (fname && !(freopen(fname, "r+", stdin))) exit(1);
	while (fgets(lbuf, sizeof(lbuf), stdin))
		for (cnt = 0, p = lbuf; (ch = *p) != '\0'; ++p, ++cnt) {
			if (cnt == 79 || ch == '\n') {
				for (; cnt < 79; ++cnt)
					putc(' ', fp);
				putc('\r', fp);
				putc('\n', fp);
				cnt = 0;
			} else {
			      putc(ch, fp);
			}
		}
    (void)fprintf(fp, "\r\n");
	rewind(fp);

    if (fstat(fd, &sbuf)) exit(1);
	mbufsize = sbuf.st_size;
    if (!(mbuf = malloc((u_int)mbufsize))) exit(1);
    if ((int)fread(mbuf, sizeof(*mbuf), mbufsize, fp) != (int)mbufsize) exit(1);
	(void)close(fd);
}

char *
ttymsg(iov, iovcnt, line, tmout)
	struct iovec *iov;
	int iovcnt;
	char *line;
	int tmout;
{
	static char device[MAXNAMLEN] = _PATH_DEV;
	static char errbuf[1024];
	register int cnt, fd, left, wret;
	struct iovec localiov[6];
	int forked = 0;

    if ((int)iovcnt > (int)sizeof(localiov) / (int)sizeof(localiov[0]))
		return ("too many iov's (change code in wall/ttymsg.c)");

	(void) strcpy(device + sizeof(_PATH_DEV) - 1, line);
	if (strchr(device + sizeof(_PATH_DEV) - 1, '/')) {
		/* A slash is an attempt to break security... */
#ifdef __linux__
		(void) sprintf(errbuf, "'/' in \"%s\"", device);
#else
		(void) snprintf(errbuf, sizeof(errbuf), "'/' in \"%s\"",
		    device);
#endif
		return (errbuf);
	}

	/*
	 * open will fail on slip lines or exclusive-use lines
	 * if not running as root; not an error.
	 */
	if ((fd = open(device, O_WRONLY|O_NONBLOCK, 0)) < 0) {
		if (errno == EBUSY || errno == EACCES)
			return (NULL);
#ifdef __linux__
		(void) sprintf(errbuf,
		    "%s: %s", device, strerror(errno));
#else
		(void) snprintf(errbuf, sizeof(errbuf),
		    "%s: %s", device, strerror(errno));
#endif
		return (errbuf);
	}

	for (cnt = left = 0; cnt < iovcnt; ++cnt)
		left += iov[cnt].iov_len;

	for (;;) {
		wret = writev(fd, iov, iovcnt);
		if (wret >= left)
			break;
		if (wret >= 0) {
			left -= wret;
			if (iov != localiov) {
				bcopy(iov, localiov,
				    iovcnt * sizeof(struct iovec));
				iov = localiov;
			}
            for (cnt = 0; (int)wret >= (int)iov->iov_len; ++cnt) {
				wret -= iov->iov_len;
				++iov;
				--iovcnt;
			}
			if (wret) {
				iov->iov_base += wret;
				iov->iov_len -= wret;
			}
			continue;
		}
		if (errno == EWOULDBLOCK) {
			int cpid, off = 0;

			if (forked) {
				(void) close(fd);
				_exit(1);
			}
			cpid = fork();
			if (cpid < 0) {
#ifdef __linux__
				(void) sprintf(errbuf,
				    "fork: %s", strerror(errno));
#else
				(void) snprintf(errbuf, sizeof(errbuf),
				    "fork: %s", strerror(errno));
#endif
				(void) close(fd);
				return (errbuf);
			}
			if (cpid) {	/* parent */
				(void) close(fd);
				return (NULL);
			}
			forked++;
			/* wait at most tmout seconds */
			(void) signal(SIGALRM, SIG_DFL);
			(void) signal(SIGTERM, SIG_DFL); /* XXX */
			(void) sigsetmask(0);
			(void) alarm((u_int)tmout);
			(void) fcntl(fd, O_NONBLOCK, &off);
			continue;
		}
		/*
		 * We get ENODEV on a slip line if we're running as root,
		 * and EIO if the line just went away.
		 */
		if (errno == ENODEV || errno == EIO)
			break;
		(void) close(fd);
		if (forked)
			_exit(1);
#ifdef __linux__
		(void) sprintf(errbuf,
		    "%s: %s", device, strerror(errno));
#else
		(void) snprintf(errbuf, sizeof(errbuf),
		    "%s: %s", device, strerror(errno));
#endif
		return (errbuf);
	}

	(void) close(fd);
	if (forked)
		_exit(0);
	return (NULL);
}

