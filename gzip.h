/*
 * tailor.h -- target dependent definitions Copyright (C) 1992-1993 Jean-loup
 * Gailly. This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, see the file COPYING.
 */

/*
 * The target dependent definitions should be defined here only. The target
 * dependent functions should be defined in tailor.c.
 */

/* $Id: gzip.h,v 1.1 1994/11/12 19:16:13 sev Exp sev $ */

#if defined(__MSDOS__) && !defined(MSDOS)
#define MSDOS
#endif

#ifdef MSDOS
#define MAXSEG_64K
#define PATH_SEP '\\'
#define OTHER_PATH_SEP ':'
#define MAX_PATH_LEN  128
#define PROTO
#define NO_SIZE_CHECK
#define casemap(c) tolow(c)	  /* Force file names to lower case */
#include <io.h>
#define OS_CODE  0x000
#else
#define near
#endif

#ifndef PATH_SEP
#define PATH_SEP '/'
#endif

#ifndef OS_CODE
#define OS_CODE  0x300		  /* assume Unix */
#endif

#ifndef MAXSEG_64K
#define fcalloc(items,size) malloc((unsigned)(items)*(unsigned)(size))
#define fcfree(ptr) free(ptr)
#else
#ifdef __TURBOC__
#include <alloc.h>
#define DYN_ALLOC
/* Turbo C 2.0 does not accept static allocations of large arrays */
void *fcalloc(unsigned items, unsigned size);
void fcfree(void *ptr);

#else				/* MSC */
#include <malloc.h>
#define fcalloc(nitems,itemsize) halloc((long)(nitems),(itemsize))
#define fcfree(ptr) hfree(ptr)
#endif
#endif

#ifndef casemap
#define casemap(c) (c)
#endif

/* Wild card expansion */
#ifdef __EMX__
#define EXPAND(argc,argv) {_response(&argc, &argv); _wildcard(&argc, &argv);}
#endif

#ifndef EXPAND
#define EXPAND(argc,argv)
#endif

/* Force binary mode on open file */
#if defined(MSDOS) || defined(OS2)
#define SET_BINARY_MODE(fd) setmode(fd, O_BINARY)
#else
#define SET_BINARY_MODE(fd)
#endif

#ifndef RECORD_IO
#define RECORD_IO 0
#endif

/* Use ASM code by default on MSDOS */
#if defined(MSDOS) && !defined(NO_ASM) && !defined(ASMV)
#define ASMV
#endif
#if defined(ASMV) && !defined(MSDOS) && defined(DYN_ALLOC)
error:DYN_ALLOC not yet supported in match.s
#endif


/*
 * gzip.h -- common declarations for all gzip modules Copyright (C) 1992-1993
 * Jean-loup Gailly. This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License, see the file
 * COPYING.
 */

#if defined(__STDC__) || defined(PROTO)
#define OF(args)  args
#else
#define OF(args)  ()
#endif

#define local static

typedef unsigned char uch;
typedef unsigned short ush;
typedef unsigned long ulg;

/* Return codes from gzip */
#define OK      0
#define ERROR   1
#define WARNING 2

/* Compression methods (see algorithm.doc) */
#define STORED     0
/* methods 3 to 7 reserved */
#define DEFLATED   8
extern int method;		  /* compression method */

/*
 * To save memory for 16 bit systems, some arrays are overlayed between the
 * various modules: deflate:  prev+head   window      d_buf  l_buf  outbuf
 * unlzw:    tab_prefix  tab_suffix  stack  inbuf  outbuf inflate:
 * window             inbuf unpack:               window             inbuf
 * For compression, input is done in window[]. For decompression, output is
 * done in window except for unlzw.
 */

#ifndef	INBUFSIZ
#define INBUFSIZ  0x8000	  /* input buffer size */
#endif
#define INBUF_EXTRA  64		  /* required by unlzw() */

#ifndef	OUTBUFSIZ
#define OUTBUFSIZ  16384	  /* output buffer size */
#endif
#define OUTBUF_EXTRA 2048	  /* required by unlzw() */

#define DIST_BUFSIZE 0x8000	  /* buffer for distances, see trees.c */

#ifdef DYN_ALLOC
#define EXTERN(type, array)  extern type * near array
#define DECLARE(type, array, size)  type * near array
#define ALLOC(type, array, size) { \
      array = (type*)fcalloc((unsigned)(((size)+1L)/2), 2*sizeof(type)); \
      if (array == NULL) error("insufficient memory"); \
   }
#define FREE(array) {if (array != NULL) fcfree(array), array=NULL;}
#else
#define EXTERN(type, array)  extern type array[]
#define DECLARE(type, array, size)  type array[size]
#define ALLOC(type, array, size)
#define FREE(array)
#endif

EXTERN(uch, inbuf);		  /* input buffer */
EXTERN(uch, outbuf);		  /* output buffer */
EXTERN(ush, d_buf);		  /* buffer for distances, see trees.c */
EXTERN(uch, window);		  /* Sliding window and suffix table (unlzw) */
#define tab_suffix window
#ifndef MAXSEG_64K
#define tab_prefix prev		  /* hash link (see deflate.c) */
#define head (prev+WSIZE)	  /* hash head (see deflate.c) */
EXTERN(ush, tab_prefix);	  /* prefix code (see unlzw.c) */
#else
#define tab_prefix0 prev
#define head tab_prefix1
EXTERN(ush, tab_prefix0);	  /* prefix for even codes */
EXTERN(ush, tab_prefix1);	  /* prefix for odd  codes */
#endif

extern unsigned insize;		  /* valid bytes in inbuf */
extern unsigned inptr;		  /* index of next byte to be processed in
				   * inbuf */
extern unsigned outcnt;		  /* bytes in output buffer */

extern long bytes_in;		  /* number of input bytes */
extern long bytes_out;		  /* number of output bytes */
extern long overhead;		  /* number of bytes in gzip header */

#define isize bytes_in
/* for compatibility with old zip sources (to be cleaned) */

extern int ifd;			  /* input file descriptor */
extern int ofd;			  /* output file descriptor */
extern char ifname[];		  /* input filename or "stdin" */
extern char ofname[];		  /* output filename or "stdout" */

extern ulg time_stamp;		  /* original time stamp (modification time) */

extern int exit_code;		  /* program exit code */

typedef int file_t;		  /* Do not use stdio */

#define NO_FILE  (-1)		  /* in memory compression */


#define	GZIP_MAGIC     "\037\213" /* Magic header for gzip files, 1F 8B */
#define	OLD_GZIP_MAGIC "\037\236" /* Magic header for gzip 0.5 = freeze 1.x */

/* gzip flag byte */
#define ORIG_NAME    0x08	  /* bit 3 set: original file name present */

/* internal file attribute */
#define UNKNOWN (-1)
#define BINARY  0
#define ASCII   1

#ifndef WSIZE
#define WSIZE 0x8000		  /* window size--must be a power of two, and */
#endif				/* at least 32K for zip's deflate method */

#define MIN_MATCH  3
#define MAX_MATCH  258
/* The minimum and maximum match lengths */

#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)

/*
 * Minimum amount of lookahead, except at the end of the input file. See
 * deflate.c for comments about the MIN_MATCH+1.
 */

#define MAX_DIST  (WSIZE-MIN_LOOKAHEAD)

/*
 * In order to simplify the code, particularly on 16 bit machines, match
 * distances are limited to MAX_DIST instead of WSIZE.
 */

extern int save_orig_name;	  /* set if original name must be saved */
extern int level;		  /* compression level */

/*
 * put_byte is used for the compressed output, put_char for the uncompressed
 * output. However unlzw() uses window for its suffix table instead of its
 * output buffer, so it does not use put_char. (to be cleaned up).
 */
#define put_byte(c) {outbuf[outcnt++]=(uch)(c); if (outcnt==OUTBUFSIZ)\
   flush_outbuf();}

/* Output a 16 bit value, lsb first */
#define put_short(w) \
{ if (outcnt < OUTBUFSIZ-2) { \
    outbuf[outcnt++] = (uch) ((w) & 0xff); \
    outbuf[outcnt++] = (uch) ((ush)(w) >> 8); \
  } else { \
    put_byte((uch)((w) & 0xff)); \
    put_byte((uch)((ush)(w) >> 8)); \
  } \
}

/* Output a 32 bit value to the bit stream, lsb first */
#define put_long(n) { \
    put_short((n) & 0xffff); \
    put_short(((ulg)(n)) >> 16); \
}

#define seekable()    0		  /* force sequential output */
#define translate_eol 0		  /* no option -a yet */

#define tolow(c)  (isupper(c) ? (c)-'A'+'a' : (c))	/* force to lower case */

#define Assert(cond,msg)
#define Trace(x)
#define Tracev(x)
#define Tracevv(x)
#define Tracec(c,x)
#define Tracecv(c,x)

/* in zip.c: */
extern void zip OF((int in, int out));
extern int file_read OF((char *buf, unsigned size));

/* in deflate.c */
void lm_init OF((int pack_level, ush * flags));
ulg deflate OF((void));

/* in trees.c */
void ct_init OF((ush * attr, int *method));
int ct_tally OF((int dist, int lc));
ulg flush_block OF((char *buf, ulg stored_len, int eof));

/* in bits.c */
void bi_init OF((file_t zipfile));
void send_bits OF((int value, int length));
unsigned bi_reverse OF((unsigned value, int length));
void bi_windup OF((void));
void copy_block OF((char *buf, unsigned len, int header));
extern int (*read_buf) OF((char *buf, unsigned size));

/* in util.c: */
extern ulg updcrc OF((uch * s, unsigned n));
extern void clear_bufs OF((void));
extern void flush_outbuf OF((void));
extern char *basename OF((char *fname));
extern void error OF((char *m));
extern void warn OF((char *a, char *b));
extern void write_error OF((void));


/*
 * lzw.h -- define the lzw functions. Copyright (C) 1992-1993 Jean-loup
 * Gailly. This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, see the file COPYING.
 */

#ifndef BITS
#define BITS 16
#endif
