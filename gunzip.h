/*
 * tailor.h -- target dependent definitions Copyright (C) 1992-1993 Jean-loup
 * Gailly. This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, see the file COPYING.
 */

/*
 * The target dependent definitions should be defined here only. The target
 * dependent functions should be defined in tailor.c.
 */

/* $Id: gunzip.h,v 1.1 1994/11/12 19:20:53 sev Exp $ */

#if defined(__MSDOS__) && !defined(MSDOS)
#define MSDOS
#endif

#ifdef MSDOS
#define MAXSEG_64K
#define PATH_SEP '\\'
#define OTHER_PATH_SEP ':'
#define MAX_PATH_LEN  128
#define PROTO
#include <io.h>
#define OS_CODE  0x000
#else
#define near
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

#ifndef OS_CODE
#define OS_CODE  0x300		  /* assume Unix */
#endif

#ifndef PATH_SEP
#define PATH_SEP '/'
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

#define memzero(s, n)     memset ((s), 0, (n))

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
int method;			  /* compression method */

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

#ifdef DYN_ALLOC
#define EXTERN(type, array)  type * near array
#define DECLARE(type, array, size)  type * near array
#define ALLOC(type, array, size) { \
      array = (type*)fcalloc((unsigned)(((size)+1L)/2), 2*sizeof(type)); \
      if (array == NULL) error("insufficient memory"); \
   }
#define FREE(array) {if (array != NULL) fcfree(array), array=NULL;}
#else
#define EXTERN(type, array)  type array[]
#define DECLARE(type, array, size)  type array[size]
#define ALLOC(type, array, size)
#define FREE(array)
#endif

EXTERN(uch, inbuf);		  /* input buffer */
EXTERN(uch, outbuf);		  /* output buffer */
EXTERN(uch, window);		  /* Sliding window and suffix table (unlzw) */

unsigned insize;		  /* valid bytes in inbuf */
unsigned inptr;			  /* index of next byte to be processed in
				   * inbuf */
unsigned outcnt;		  /* bytes in output buffer */

long bytes_in;			  /* number of input bytes */
long bytes_out;			  /* number of output bytes */

int ifd;			  /* input file descriptor */
int ofd;			  /* output file descriptor */
char ifname[];			  /* input filename or "stdin" */
char ofname[];			  /* output filename or "stdout" */

int exit_code;			  /* program exit code */

typedef int file_t;		  /* Do not use stdio */

#define	GZIP_MAGIC     "\037\213" /* Magic header for gzip files, 1F 8B */
#define	OLD_GZIP_MAGIC "\037\236" /* Magic header for gzip 0.5 = freeze 1.x */

/* gzip flag byte */
#define CONTINUATION 0x02	  /* bit 1 set: continuation of multi-part
				   * gzip file */
#define EXTRA_FIELD  0x04	  /* bit 2 set: extra field present */
#define ORIG_NAME    0x08	  /* bit 3 set: original file name present */
#define COMMENT      0x10	  /* bit 4 set: file comment present */
#define ENCRYPTED    0x20	  /* bit 5 set: file is encrypted */
#define RESERVED     0xC0	  /* bit 6,7:   reserved */

#ifndef WSIZE
#define WSIZE 0x8000		  /* window size--must be a power of two, and */
#endif				/* at least 32K for zip's deflate method */

int verbose;			  /* be verbose (-v) */
int test;			  /* check .z file integrity */
int to_stdout;			  /* output to stdout (-c) */

#define get_byte()  (inptr < insize ? inbuf[inptr++] : fill_inbuf())

#define put_char(c) {window[outcnt++]=(uch)(c); if (outcnt==WSIZE)\
   flush_window();}

/* Macros for getting two-byte and four-byte header values */
#define SH(p) ((ush)(uch)((p)[0]) | ((ush)(uch)((p)[1]) << 8))
#define LG(p) ((ulg)(SH(p)) | ((ulg)(SH((p)+2)) << 16))

/* Diagnostic functions */
#define Tracecv(c, x)
#define Tracevv(x)

/* in unzip.c */
void unzip OF((int in, int out));

/* in util.c: */
ulg updcrc OF((uch * s, unsigned n));
void clear_bufs OF((void));
int fill_inbuf OF((void));
void flush_window OF((void));
char *basename OF((char *fname));
void error OF((char *m));
void read_error OF((void));
void write_error OF((void));

/* in inflate.c */
int inflate OF((void));


/*
 * lzw.h -- define the lzw functions. Copyright (C) 1992-1993 Jean-loup
 * Gailly. This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, see the file COPYING.
 */

#ifndef BITS
#define BITS 16
#endif
