/* $Id: crt.h,v 1.6 1995/06/05 23:28:45 tom Exp $
 *
 * Title:	crt.h
 * Author:	Thomas E. Dickey
 * Created:	24 Aug 1984, broke out of 'flist.h'
 * Last update:
 *		27 May 1995, ifdef-guard
 *
 *	Buffer limits and other special things needed to interface with the
 *	'crt.c' screen-handling module.
 */

#ifndef CRT_H
#define CRT_H

#define	ishigh(c)	(0x80 & c)	/* Test highlighting flag	*/
#define	tohigh(c)	(0x80 | c)	/* Set highlighting flag	*/

#ifndef max
#define	max(a,b)	(((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define	min(a,b)	(((a) < (b)) ? (a) : (b))
#endif

#undef BYTE
typedef	unsigned char BYTE;

#undef WORD
typedef	unsigned int WORD;

/*
 * Buffer limits:
 */
#define	CRT_COLS	133	/* Length of screen line-buffers	*/
#define	CRT_LINES	256	/* Allow a lot of lines			*/

extern	char	*crtvec[];

extern	void	crt__ED (int y, int x);
extern	void	crt__EL (int y, int x);
extern	void	crt__NL0 (int flg);
extern	int	crt_ansi (void);
extern	void	crt_clear (void);
extern	int	crt_end (void);
extern	int	crt_endm (void);
extern	void	crt_help (char *library, char *program);
extern	void	crt_high (char *s_, int len);
extern	void	crt_init (int (*func)(short *lpp_, short *width_));
extern	int	crt_lpp (void);
extern	void	crt_margin (int lo, int hi);
extern	void	crt_move (int y, int x);
extern	int	crt_qsgr(int inx);
extern	void	crt_quit (int erase);
extern	void	crt_refresh (void);
extern	void	crt_reset (void);
extern	int	crt_scroll (int iline, int numlines, void (*func)(int));
extern	int	crt_set (int top, int val);
extern	void	crt_text (char* bfr, int line, int mode);
extern	int	crt_top (void);
extern	int	crt_topm (void);
extern	int	crt_vt100 (void);
extern	int	crt_vt52 (void);
extern	int	crt_width (void);
extern	int	crt_x (void);
extern	int	crt_y (void);
extern	void	putraw (char *s_);
extern	void	snapshot (void);
extern	void	sound_alarm (void);
extern	int	termsize (int *width_, int *length_);

#endif /* CRT_H */
