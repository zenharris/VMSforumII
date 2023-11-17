
#include	<starlet.h>
#include	<lib$routines.h>
#include	<dcdef.h>
#include	<ttdef.h>
#include	<tt2def.h>

#include	<descrip.h>
#include	<iodef.h>
#include	<stsdef.h>

#include        ints
#include	"extools.h"

#define	OK(f)	$VMS_STATUS_SUCCESS(status=f)
#define	SYS(f)	if (!OK(f)) lib$stop(status)
#define	QIO(mask)	sys$qiow (\
			0,		/* Event flag number		*/\
			tty_chan,\
			mask,\
			&iosb,0,0,\
			&cbfr,		/* buffer address		*/\
			sizeof(cbfr),	/* buffer size			*/\
			0,		/* P3 -- speed specifier	*/\
			0,		/* P4 -- fill specifier		*/\
			0,		/* P5 -- parity flags		*/\
			0)		/* prompt-string buffer size	*/


static	$DESCRIPTOR(tty_name,"SYS$COMMAND");
static	unsigned short	tty_chan = 0;

int
termsize (int *width_, int *length_)
{
   int     status;

	struct	{
		short	sts,
			count;
		unsigned device;
		}	iosb;

	struct	{
		unsigned
		char	class,
			type;
		short	width;
		unsigned ttdef,
			tt2def;
		}	cbfr;

	SYS(sys$assign(&tty_name, &tty_chan, 0,0));

	SYS(QIO(IO$_SENSEMODE));

	*width_	= cbfr.width;
	*length_ = cbfr.ttdef >> 24;  /* >> TT$V_PAGE;  */

	return (1);
}
