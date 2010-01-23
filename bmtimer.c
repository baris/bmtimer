/*
 * Baris Metin <baris@gelecek.com.tr>
 */

#include <dockapp.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

#include "bmtimer.xpm"

char *displayName = "";

static DAProgramOption options[] = {
    {"-display", NULL, "display to use", DOString, False, {&displayName} }
};

int
main(int argc, char **argv)
{
    Pixmap pixmap;
    GC gc;
    int sec, min, hr;
    struct passwd *login;
    char *hostname = (char *) malloc(40);

    if ((gethostname(hostname, 40)) != 0) {
	    perror("gethostname()");
	    exit(EXIT_FAILURE);
    }

    if ((login = getpwuid(getuid())) == NULL) {
	perror ("getlogin()");
	exit(EXIT_FAILURE);
    }

    DAParseArguments(argc, argv, options,
		     sizeof(options)/sizeof(DAProgramOption),
		     "BM Timer", "bmtimer 0.1");

    DAInitialize(displayName, "BMTimer", 55, 55, argc, argv);

    pixmap = DAMakePixmap();
    gc = DefaultGC(DADisplay, DefaultScreen(DADisplay));

    DAShow();

    sec = min = hr = 0;
    while (1) {
	XEvent ev;
	char buf[20];

	XSetForeground(DADisplay, gc, 
		       BlackPixel(DADisplay, DefaultScreen(DADisplay)));
	XFillRectangle(DADisplay, pixmap, gc, 0, 0, 55, 55);

	XSetForeground(DADisplay, gc, bmtimer_xpm);

	/* user@hostname */
	sprintf(buf, "%s", login->pw_name);
	XDrawString(DADisplay, pixmap, gc, 12, 14, buf, strlen(buf));
	sprintf(buf, "@");
	XDrawString(DADisplay, pixmap, gc, 23, 24, buf, strlen(buf));
	sprintf(buf, "%s", hostname);
	XDrawString(DADisplay, pixmap, gc, 12, 34, buf, strlen(buf));

	/* time */
	if (sec == 60) { min ++; sec=0; }
	if (min == 60) { hr ++; min=0; }
	sprintf(buf, "%02d:%02d:%02d", hr, min, sec);
	XDrawString(DADisplay, pixmap, gc, 5, 52, buf, strlen(buf));
	

	DASetPixmap(pixmap);

	/* handle pending X events */
	while (XPending(DADisplay)) {
	    XNextEvent(DADisplay, &ev);
	    DAProcessEvent(&ev);
	}

	sleep(1);
	sec++;
    }

    return 0;
}

