#include "gzip.h"

#include <stdio.h>

#ifndef MAX_PATH_LEN
#define MAX_PATH_LEN   1024	  /* max pathname length */
#endif

DECLARE(uch, inbuf, INBUFSIZ + INBUF_EXTRA);
DECLARE(uch, outbuf, OUTBUFSIZ + OUTBUF_EXTRA);
DECLARE(ush, d_buf, DIST_BUFSIZE);
DECLARE(uch, window, 2L * WSIZE);
#ifndef MAXSEG_64K
DECLARE(ush, tab_prefix, 1L << BITS);
#else
DECLARE(ush, tab_prefix0, 1L << (BITS - 1));
DECLARE(ush, tab_prefix1, 1L << (BITS - 1));
#endif
/* local variables */

int method = DEFLATED;		  /* compression method */
int level = 9;			  /* compression level */
int exit_code = OK;		  /* program exit code */
int save_orig_name;		  /* set if original name must be saved */
ulg time_stamp;			  /* original time stamp (modification time) */

long bytes_in;			  /* number of input bytes */
long bytes_out;			  /* number of output bytes */
char ifname[MAX_PATH_LEN];	  /* input filename */
char ofname[MAX_PATH_LEN];	  /* output filename */
int remove_ofname = 0;		  /* remove output file on error */
int ifd;			  /* input file descriptor */
int ofd;			  /* output file descriptor */
unsigned insize;		  /* valid bytes in inbuf */
unsigned inptr;			  /* index of next byte to be processed in
				   * inbuf */
unsigned outcnt;		  /* bytes in output buffer */


gzip(ifile, ofile)
char *ifile, *ofile;
{
  FILE *in, *out;

  in = fopen(ifile, "r");

  out = fopen(ofile, "w");
  /* Allocate all global buffers (for DYN_ALLOC option) */
  ALLOC(uch, inbuf, INBUFSIZ + INBUF_EXTRA);
  ALLOC(uch, outbuf, OUTBUFSIZ + OUTBUF_EXTRA);
  ALLOC(ush, d_buf, DIST_BUFSIZE);
  ALLOC(uch, window, 2L * WSIZE);
#ifndef MAXSEG_64K
  ALLOC(ush, tab_prefix, 1L << BITS);
#else
  ALLOC(ush, tab_prefix0, 1L << (BITS - 1));
  ALLOC(ush, tab_prefix1, 1L << (BITS - 1));
#endif

  clear_bufs();
  zip(fileno(in), fileno(out));
  fclose(in);
  fclose(out);
}

/*
 * ========================================================================
 * Free all dynamically allocated variables and exit with the given code.
 */
local void do_exit(exitcode)
int exitcode;
{
  FREE(inbuf);
  FREE(outbuf);
  FREE(d_buf);
  FREE(window);
#ifndef MAXSEG_64K
  FREE(tab_prefix);
#else
  FREE(tab_prefix0);
  FREE(tab_prefix1);
#endif
  exit(exitcode);
}

/*
 * ========================================================================
 * Signal and error handler.
 */
abort_gzip()
{
  if (remove_ofname)
  {
    close(ofd);
    unlink(ofname);
  }
  do_exit(ERROR);
}

/*
 * bits.c -- output variable-length bit strings Copyright (C) 1992-1993
 * Jean-loup Gailly This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License, see the file
 * COPYING.
 */


/*
 * PURPOSE
 * 
 * Output variable-length bit strings. Compression can be done to a file or to
 * memory. (The latter is not supported in this version.)
 * 
 * DISCUSSION
 * 
 * The PKZIP "deflate" file format interprets compressed file data as a sequence
 * of bits.  Multi-bit strings in the file may cross byte boundaries without
 * restriction.
 * 
 * The first bit of each byte is the low-order bit.
 * 
 * The routines in this file allow a variable-length bit value to be output
 * right-to-left (useful for literal values). For left-to-right output
 * (useful for code strings from the tree routines), the bits must have been
 * reversed first with bi_reverse().
 * 
 * For in-memory compression, the compressed bit stream goes directly into the
 * requested output buffer. The input data is read in blocks by the
 * mem_read() function. The buffer is limited to 64K on 16 bit machines.
 * 
 * INTERFACE
 * 
 * void bi_init (FILE *zipfile) Initialize the bit string routines.
 * 
 * void send_bits (int value, int length) Write out a bit string, taking the
 * source bits right to left.
 * 
 * int bi_reverse (int value, int length) Reverse the bits of a bit string,
 * taking the source bits left to right and emitting them right to left.
 * 
 * void bi_windup (void) Write out any remaining bits in an incomplete byte.
 * 
 * void copy_block(char *buf, unsigned len, int header) Copy a stored block to
 * the zip file, storing first the length and its one's complement if
 * requested.
 * 
 * static char rcsid[] = "$Id: gzip.c,v 1.2 1994/11/15 14:40:06 sev Exp $";
 */

/*
 * ===========================================================================
 * Local data used by the "bit string" routines.
 */

local file_t zfile;		  /* output gzip file */

local unsigned short bi_buf;

/*
 * Output buffer. bits are inserted starting at the bottom (least significant
 * bits).
 */

#define Buf_size (8 * 2*sizeof(char))

/*
 * Number of bits used within bi_buf. (bi_buf might be implemented on more
 * than 16 bits on some systems.)
 */

local int bi_valid;

/*
 * Number of valid bits in bi_buf.  All bits above the last valid bit are
 * always zero.
 */

int (*read_buf) OF((char *buf, unsigned size)) = file_read;

/* Current input function. Set to mem_read for in-memory compression */

/*
 * ===========================================================================
 * Initialize the bit string routines.
 */
void bi_init(zipfile)
file_t zipfile;			  /* output zip file, NO_FILE for in-memory
				   * compression */
{
  zfile = zipfile;
  bi_buf = 0;
  bi_valid = 0;

  /*
   * Set the defaults for file compression. They are set by memcompress for
   * in-memory compression.
   */
  if (zfile != NO_FILE)
  {
    read_buf = file_read;
  }
}

/*
 * ===========================================================================
 * Send a value on a given number of bits. IN assertion: length <= 16 and
 * value fits in length bits.
 */
void send_bits(value, length)
int value;			  /* value to send */
int length;			  /* number of bits */
{

  /*
   * If not enough room in bi_buf, use (valid) bits from bi_buf and (16 -
   * bi_valid) bits from value, leaving (width - (16-bi_valid)) unused bits
   * in value.
   */
  if (bi_valid > (int) Buf_size - length)
  {
    bi_buf |= (value << bi_valid);
    put_short(bi_buf);
    bi_buf = (ush) value >> (Buf_size - bi_valid);
    bi_valid += length - Buf_size;
  }
  else
  {
    bi_buf |= value << bi_valid;
    bi_valid += length;
  }
}

/*
 * ===========================================================================
 * Reverse the first len bits of a code, using straightforward code (a faster
 * method would use a table) IN assertion: 1 <= len <= 15
 */
unsigned bi_reverse(code, len)
unsigned code;			  /* the value to invert */
int len;			  /* its bit length */
{
  register unsigned res = 0;

  do
  {
    res |= code & 1;
    code >>= 1, res <<= 1;
  } while (--len > 0);
  return res >> 1;
}

/*
 * ===========================================================================
 * Write out any remaining bits in an incomplete byte.
 */
void bi_windup()
{
  if (bi_valid > 8)
  {
    put_short(bi_buf);
  }
  else if (bi_valid > 0)
  {
    put_byte(bi_buf);
  }
  bi_buf = 0;
  bi_valid = 0;
}

/*
 * ===========================================================================
 * Copy a stored block to the zip file, storing first the length and its
 * one's complement if requested.
 */
void copy_block(buf, len, header)
char *buf;			  /* the input data */
unsigned len;			  /* its length */
int header;			  /* true if block header must be written */
{
  bi_windup();			  /* align on byte boundary */

  if (header)
  {
    put_short((ush) len);
    put_short((ush) ~ len);
  }
  while (len--)
    put_byte(*buf++);
}


/*
 * deflate.c -- compress data using the deflation algorithm Copyright (C)
 * 1992-1993 Jean-loup Gailly This is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License, see
 * the file COPYING.
 */

/*
 * PURPOSE
 * 
 * Identify new text as repetitions of old text within a fixed- length sliding
 * window trailing behind the new text.
 * 
 * DISCUSSION
 * 
 * The "deflation" process depends on being able to identify portions of the
 * input text which are identical to earlier input (within a sliding window
 * trailing behind the input currently being processed).
 * 
 * The most straightforward technique turns out to be the fastest for most input
 * files: try all possible matches and select the longest. The key feature of
 * this algorithm is that insertions into the string dictionary are very
 * simple and thus fast, and deletions are avoided completely. Insertions are
 * performed at each input character, whereas string matches are performed
 * only when the previous match ends. So it is preferable to spend more time
 * in matches to allow very fast string insertions and avoid deletions. The
 * matching algorithm for small strings is inspired from that of Rabin &
 * Karp. A brute force approach is used to find longer strings when a small
 * match has been found. A similar algorithm is used in comic (by Jan-Mark
 * Wams) and freeze (by Leonid Broukhis). A previous version of this file
 * used a more sophisticated algorithm (by Fiala and Greene) which is
 * guaranteed to run in linear amortized time, but has a larger average cost,
 * uses more memory and is patented. However the F&G algorithm may be faster
 * for some highly redundant files if the parameter max_chain_length
 * (described below) is too large.
 * 
 * ACKNOWLEDGEMENTS
 * 
 * The idea of lazy evaluation of matches is due to Jan-Mark Wams, and I found
 * it in 'freeze' written by Leonid Broukhis. Thanks to many info-zippers for
 * bug reports and testing.
 * 
 * REFERENCES
 * 
 * APPNOTE.TXT documentation file in PKZIP 1.93a distribution.
 * 
 * A description of the Rabin and Karp algorithm is given in the book
 * "Algorithms" by R. Sedgewick, Addison-Wesley, p252.
 * 
 * Fiala,E.R., and Greene,D.H. Data Compression with Finite Windows, Comm.ACM,
 * 32,4 (1989) 490-595
 * 
 * INTERFACE
 * 
 * void lm_init (int pack_level, ush *flags) Initialize the "longest match"
 * routines for a new file
 * 
 * ulg deflate (void) Processes a new input file and return its compressed
 * length. Sets the compressed length, crc, deflate flags and internal file
 * attributes.
 * 
 * static char rcsid[] = " deflate.c,v 0.10 1993/02/10 16:07:22 jloup Exp
 * $";
 */

/*
 * ===========================================================================
 * Configuration parameters
 */

/*
 * Compile with MEDIUM_MEM to reduce the memory requirements or with
 * SMALL_MEM to use as little memory as possible. Use BIG_MEM if the entire
 * input file can be held in memory (not possible on 16 bit systems).
 * Warning: defining these symbols affects HASH_BITS (see below) and thus
 * affects the compression ratio. The compressed output is still correct, and
 * might even be smaller in some cases.
 */

#ifdef SMALL_MEM
#define HASH_BITS  13		  /* Number of bits used to hash strings */
#endif
#ifdef MEDIUM_MEM
#define HASH_BITS  14
#endif
#ifndef HASH_BITS
#define HASH_BITS  15
/* For portability to 16 bit machines, do not use values above 15. */
#endif

/*
 * To save space (see unlzw.c), we overlay prev+head with tab_prefix and
 * window with tab_suffix. Check that we can do this:
 */
#if WSIZE<<1 > 1<<BITS
error:cannot overlay window with tab_suffix and prev with tab_prefix0
#endif
#if HASH_BITS > BITS-1
error:cannot overlay head with tab_prefix1
#endif

#define HASH_SIZE (unsigned)(1<<HASH_BITS)
#define HASH_MASK (HASH_SIZE-1)
#define WMASK     (WSIZE-1)
/* HASH_SIZE and WSIZE must be powers of two */

#define NIL 0
/* Tail of hash chains */

#define FAST 4
#define SLOW 2
/* speed options for the general purpose bit flag */

#ifndef TOO_FAR
#define TOO_FAR 4096
#endif
/* Matches of length 3 are discarded if their distance exceeds TOO_FAR */

/*
 * ===========================================================================
 * Local data used by the "longest match" routines.
 */

typedef ush Pos;
typedef unsigned IPos;

/*
 * A Pos is an index in the character window. We use short instead of int to
 * save space in the various tables. IPos is used only for parameter passing.
 */

/* DECLARE(uch, window, 2L*WSIZE); */

/*
 * Sliding window. Input bytes are read into the second half of the window,
 * and move to the first half later to keep a dictionary of at least WSIZE
 * bytes. With this organization, matches are limited to a distance of
 * WSIZE-MAX_MATCH bytes, but this ensures that IO is always performed with a
 * length multiple of the block size. Also, it limits the window size to 64K,
 * which is quite useful on MSDOS. To do: limit the window size to WSIZE+BSZ
 * if SMALL_MEM (the code would be less efficient).
 */

/* DECLARE(Pos, prev, WSIZE); */

/*
 * Link to older string with same hash index. To limit the size of this array
 * to 64K, this link is maintained only for the last 32K strings. An index in
 * this array is thus a window index modulo 32K.
 */

/* DECLARE(Pos, head, 1<<HASH_BITS); */
/* Heads of the hash chains or NIL. */

ulg window_size = (ulg) 2 * WSIZE;

/*
 * window size, 2*WSIZE except for MMAP or BIG_MEM, where it is the input
 * file length plus MIN_LOOKAHEAD.
 */

long block_start;

/*
 * window position at the beginning of the current output block. Gets
 * negative when the window is moved backwards.
 */

local unsigned ins_h;		  /* hash index of string to be inserted */

#define H_SHIFT  ((HASH_BITS+MIN_MATCH-1)/MIN_MATCH)

/*
 * Number of bits by which ins_h and del_h must be shifted at each input
 * step. It must be such that after MIN_MATCH steps, the oldest byte no
 * longer takes part in the hash key, that is: H_SHIFT * MIN_MATCH >=
 * HASH_BITS
 */

unsigned int near prev_length;

/*
 * Length of the best match at previous step. Matches not greater than this
 * are discarded. This is used in the lazy match evaluation.
 */

unsigned near strstart;		  /* start of string to insert */
unsigned near match_start;	  /* start of matching string */
local int eofile;		  /* flag set at end of input file */
local unsigned lookahead;	  /* number of valid bytes ahead in window */

unsigned near max_chain_length;

/*
 * To speed up deflation, hash chains are never searched beyond this length.
 * A higher limit improves compression ratio but degrades the speed.
 */

local unsigned int max_lazy_match;

/*
 * Attempt to find a better match only when the current match is strictly
 * smaller than this value.
 */

int near good_match;

/* Use a faster search when the previous match is longer than this */


/*
 * Values for max_lazy_match, good_match and max_chain_length, depending on
 * the desired pack level (0..9). The values given below have been tuned to
 * exclude worst case performance for pathological files. Better values may
 * be found for specific files.
 */

typedef struct config
{
  ush good_length;		  /* reduce lazy search above this match
				   * length */
  ush max_lazy;			  /* do not perform lazy search above this
				   * match length */
  ush nice_length;		  /* quit search above this match length */
  ush max_chain;
} config;

#ifdef  FULL_SEARCH
#define nice_match MAX_MATCH
#else
int near nice_match;		  /* Stop searching when current match
				   * exceeds this */

#endif

local config configuration_table[10] =
{
/* good lazy nice chain */
   /* 0 */ 
 {0, 0, 0, 0},			  /* store only */
  /* 1 */ 
 {4, 4, 16, 16},		  /* maximum speed */
  /* 2 */ 
 {6, 8, 16, 16},
  /* 3 */ 
 {8, 16, 32, 32},
  /* 4 */ 
 {8, 16, 64, 64},
  /* 5 */ 
 {8, 16, 128, 128},
  /* 6 */ 
 {8, 32, 128, 256},
  /* 7 */ 
 {8, 64, 128, 512},
  /* 8 */ 
 {32, 128, 258, 1024},
  /* 9 */ 
 {32, 258, 258, 4096}};		  /* maximum compression */

/*
 * Note: the current code requires max_lazy >= MIN_MATCH and max_chain >= 4
 * but these restrictions can easily be removed at a small cost.
 */

/*
 * ===========================================================================
 * Prototypes for local functions.
 */
local void fill_window OF((void));
int longest_match OF((IPos cur_match));

/*
 * ===========================================================================
 * Update a hash value with the given input byte IN  assertion: all calls to
 * to UPDATE_HASH are made with consecutive input characters, so that a
 * running hash key can be computed from the previous key instead of complete
 * recalculation each time.
 */
#define UPDATE_HASH(h,c) (h = (((h)<<H_SHIFT) ^ (c)) & HASH_MASK)

/*
 * ===========================================================================
 * Insert string s in the dictionary and set match_head to the previous head
 * of the hash chain (the most recent string with same hash key). Return the
 * previous length of the hash chain. IN  assertion: all calls to to
 * INSERT_STRING are made with consecutive input characters and the first
 * MIN_MATCH bytes of s are valid (except for the last MIN_MATCH-1 bytes of
 * the input file).
 */
#define INSERT_STRING(s, match_head) \
   (UPDATE_HASH(ins_h, window[(s) + MIN_MATCH-1]), \
    prev[(s) & WMASK] = match_head = head[ins_h], \
    head[ins_h] = (s))

/*
 * ===========================================================================
 * Initialize the "longest match" routines for a new file
 */
void lm_init(pack_level, flags)
int pack_level;			  /* 0: store, 1: best speed, 9: best
				   * compression */
ush *flags;			  /* general purpose bit flag */
{
  register unsigned j;

  if (pack_level < 1 || pack_level > 9)
    error("bad pack level");

  /* Initialize the hash table. */
  for (j = 0; j < HASH_SIZE; j++)
    head[j] = NIL;
  /* prev will be initialized on the fly */

  /*
   * Set the default configuration parameters:
   */
  max_lazy_match = configuration_table[pack_level].max_lazy;
  good_match = configuration_table[pack_level].good_length;
#ifndef FULL_SEARCH
  nice_match = configuration_table[pack_level].nice_length;
#endif
  max_chain_length = configuration_table[pack_level].max_chain;
  if (pack_level == 1)
  {
    *flags |= FAST;
  }
  else if (pack_level == 9)
  {
    *flags |= SLOW;
  }
  /* ??? reduce max_chain_length for binary files */

  strstart = 0;
  block_start = 0L;

#ifdef MAXSEG_64K
  /* Can't read a 64K block */
  lookahead = read_buf((char *) window, (unsigned) WSIZE);
#else
  lookahead = read_buf((char *) window, 2 * WSIZE);
#endif
  if (lookahead == 0 || lookahead == (unsigned) EOF)
  {
    eofile = 1, lookahead = 0;
    return;
  }
  eofile = 0;

  /*
   * Make sure that we always have enough lookahead. This is important if
   * input comes from a device such as a tty.
   */
  while (lookahead < MIN_LOOKAHEAD && !eofile)
    fill_window();

  ins_h = 0;
  for (j = 0; j < MIN_MATCH - 1; j++)
    UPDATE_HASH(ins_h, window[j]);

  /*
   * If lookahead < MIN_MATCH, ins_h is garbage, but this is not important
   * since only literal bytes will be emitted.
   */
}

/*
 * ===========================================================================
 * Set match_start to the longest match starting at the given string and
 * return its length. Matches shorter or equal to prev_length are discarded,
 * in which case the result is equal to prev_length and match_start is
 * garbage. IN assertions: cur_match is the head of the hash chain for the
 * current string (strstart) and its distance is <= MAX_DIST, and prev_length
 * >= 1
 */

/*
 * For MSDOS, OS/2 and 386 Unix, an optimized version is in match.asm or
 * match.s. The code is functionally equivalent, so you can use the C version
 * if desired.
 */
int longest_match(cur_match)
IPos cur_match;			  /* current match */
{
  unsigned chain_length = max_chain_length;	/* max hash chain length */
  register uch *scan = window + strstart;	/* current string */
  register uch *match;		  /* matched string */
  register int len;		  /* length of current match */
  int best_len = prev_length;	  /* best match length so far */
  IPos limit = strstart > (IPos) MAX_DIST ? strstart - (IPos) MAX_DIST : NIL;

  /*
   * Stop when cur_match becomes <= limit. To simplify the code, we prevent
   * matches with the string of window index 0.
   */

  /*
   * The code is optimized for HASH_BITS >= 8 and MAX_MATCH-2 multiple of 16.
   * It is easy to get rid of this optimization if necessary.
   */
#if HASH_BITS < 8 || MAX_MATCH != 258
error:Code too clever
#endif

#ifdef UNALIGNED_OK

  /*
   * Compare two bytes at a time. Note: this is not always beneficial. Try
   * with and without -DUNALIGNED_OK to check.
   */
       register uch *strend = window + strstart + MAX_MATCH - 1;
  register ush scan_start = *(ush *) scan;
  register ush scan_end = *(ush *) (scan + best_len - 1);

#else
       register uch *strend = window + strstart + MAX_MATCH;
  register uch scan_end1 = scan[best_len - 1];
  register uch scan_end = scan[best_len];

#endif

  /* Do not waste too much time if we already have a good match: */
  if (prev_length >= good_match)
  {
    chain_length >>= 2;
  }
  Assert(strstart <= window_size - MIN_LOOKAHEAD, "insufficient lookahead");

  do
  {
    Assert(cur_match < strstart, "no future");
    match = window + cur_match;

    /*
     * Skip to next match if the match length cannot increase or if the match
     * length is less than 2:
     */
#if (defined(UNALIGNED_OK) && MAX_MATCH == 258)

    /*
     * This code assumes sizeof(unsigned short) == 2. Do not use UNALIGNED_OK
     * if your compiler uses a different size.
     */
    if (*(ush *) (match + best_len - 1) != scan_end ||
	*(ush *) match != scan_start)
      continue;

    /*
     * It is not necessary to compare scan[2] and match[2] since they are
     * always equal when the other bytes match, given that the hash keys are
     * equal and that HASH_BITS >= 8. Compare 2 bytes at a time at
     * strstart+3, +5, ... up to strstart+257. We check for insufficient
     * lookahead only every 4th comparison; the 128th check will be made at
     * strstart+257. If MAX_MATCH-2 is not a multiple of 8, it is necessary
     * to put more guard bytes at the end of the window, or to check more
     * often for insufficient lookahead.
     */
    scan++, match++;
    do
    {
    } while (*(ush *) (scan += 2) == *(ush *) (match += 2) &&
	     *(ush *) (scan += 2) == *(ush *) (match += 2) &&
	     *(ush *) (scan += 2) == *(ush *) (match += 2) &&
	     *(ush *) (scan += 2) == *(ush *) (match += 2) &&
	     scan < strend);
    /* The funny "do {}" generates better code on most compilers */

    /* Here, scan <= window+strstart+257 */
    Assert(scan <= window + (unsigned) (window_size - 1), "wild scan");
    if (*scan == *match)
      scan++;

    len = scan - strend + (MAX_MATCH - 1);
    scan = strend - (MAX_MATCH - 1);

#else				/* UNALIGNED_OK */

    if (match[best_len] != scan_end ||
	match[best_len - 1] != scan_end1 ||
	*match != *scan ||
	*++match != scan[1])
      continue;

    /*
     * The check at best_len-1 can be removed because it will be made again
     * later. (This heuristic is not always a win.) It is not necessary to
     * compare scan[2] and match[2] since they are always equal when the
     * other bytes match, given that the hash keys are equal and that
     * HASH_BITS >= 8.
     */
    scan += 2, match++;

    /*
     * We check for insufficient lookahead only every 8th comparison; the
     * 256th check will be made at strstart+258.
     */
    do
    {
    } while (*++scan == *++match && *++scan == *++match &&
	     *++scan == *++match && *++scan == *++match &&
	     *++scan == *++match && *++scan == *++match &&
	     *++scan == *++match && *++scan == *++match &&
	     scan < strend);

    len = scan - strend + MAX_MATCH;
    scan = strend - MAX_MATCH;
#endif				/* UNALIGNED_OK */

    if (len > best_len)
    {
      match_start = cur_match;
      best_len = len;
      if (len >= nice_match)
	break;
#ifdef UNALIGNED_OK
      scan_end = *(ush *) (scan + best_len - 1);
#else
      scan_end1 = scan[best_len - 1];
      scan_end = scan[best_len];
#endif
    }
  } while ((cur_match = prev[cur_match & WMASK]) > limit
	   && --chain_length != 0);

  return best_len;
}

#define check_match(start, match, length)

/*
 * ===========================================================================
 * Fill the window when the lookahead becomes insufficient. Updates strstart
 * and lookahead, and sets eofile if end of input file. IN assertion:
 * lookahead < MIN_LOOKAHEAD && strstart + lookahead > 0 OUT assertions: at
 * least one byte has been read, or eofile is set; file reads are performed
 * for at least two bytes (required for the translate_eol option).
 */
local void fill_window()
{
  register unsigned n, m;
  unsigned more = (unsigned) (window_size - (ulg) lookahead - (ulg) strstart);

  /* Amount of free space at the end of the window. */

  /*
   * If the window is almost full and there is insufficient lookahead, move
   * the upper half to the lower one to make room in the upper half.
   */
  if (more == (unsigned) EOF)
  {

    /*
     * Very unlikely, but possible on 16 bit machine if strstart == 0 and
     * lookahead == 1 (input done one byte at time)
     */
    more--;
  }
  else if (strstart >= WSIZE + MAX_DIST)
  {

    /*
     * By the IN assertion, the window is not empty so we can't confuse more
     * == 0 with more == 64K on a 16 bit machine.
     */
    Assert(window_size == (ulg) 2 * WSIZE, "no sliding with BIG_MEM");

    memcpy((char *) window, (char *) window + WSIZE, (unsigned) WSIZE);
    match_start -= WSIZE;
    strstart -= WSIZE;		  /* we now have strstart >= MAX_DIST: */

    block_start -= (long) WSIZE;

    for (n = 0; n < HASH_SIZE; n++)
    {
      m = head[n];
      head[n] = (Pos) (m >= WSIZE ? m - WSIZE : NIL);
    }
    for (n = 0; n < WSIZE; n++)
    {
      m = prev[n];
      prev[n] = (Pos) (m >= WSIZE ? m - WSIZE : NIL);

      /*
       * If n is not on any hash chain, prev[n] is garbage but its value will
       * never be used.
       */
    }
    more += WSIZE;
  }
  /* At this point, more >= 2 */
  if (!eofile)
  {
    n = read_buf((char *) window + strstart + lookahead, more);
    if (n == 0 || n == (unsigned) EOF)
    {
      eofile = 1;
    }
    else
    {
      lookahead += n;
    }
  }
}

/*
 * ===========================================================================
 * Flush the current block, with given end-of-file flag. IN assertion:
 * strstart is set to the end of the current match.
 */
#define FLUSH_BLOCK(eof) \
   flush_block(block_start >= 0L ? (char*)&window[(unsigned)block_start] : \
                (char*)NULL, (long)strstart - block_start, (eof))

/*
 * ===========================================================================
 * Processes a new input file and return its compressed length.
 */
#ifdef NO_LAZY
ulg deflate()
{
  IPos hash_head;		  /* head of the hash chain */
  int flush;			  /* set if current block must be flushed */
  unsigned match_length = 0;	  /* length of best match */

  prev_length = MIN_MATCH - 1;
  while (lookahead != 0)
  {

    /*
     * Insert the string window[strstart .. strstart+2] in the dictionary,
     * and set hash_head to the head of the hash chain:
     */
    INSERT_STRING(strstart, hash_head);

    /*
     * Find the longest match, discarding those <= prev_length. At this point
     * we have always match_length < MIN_MATCH
     */
    if (hash_head != NIL && strstart - hash_head <= MAX_DIST)
    {

      /*
       * To simplify the code, we prevent matches with the string of window
       * index 0 (in particular we have to avoid a match of the string with
       * itself at the start of the input file).
       */
      match_length = longest_match(hash_head);
      /* longest_match() sets match_start */
      if (match_length > lookahead)
	match_length = lookahead;
    }
    if (match_length >= MIN_MATCH)
    {
      check_match(strstart, match_start, match_length);

      flush = ct_tally(strstart - match_start, match_length - MIN_MATCH);

      lookahead -= match_length;
      match_length--;		  /* string at strstart already in hash table */
      do
      {
	strstart++;
	INSERT_STRING(strstart, hash_head);

	/*
	 * strstart never exceeds WSIZE-MAX_MATCH, so there are always
	 * MIN_MATCH bytes ahead. If lookahead < MIN_MATCH these bytes are
	 * garbage, but it does not matter since the next lookahead bytes
	 * will always be emitted as literals.
	 */
      } while (--match_length != 0);
    }
    else
    {
      /* No match, output a literal byte */
      flush = ct_tally(0, window[strstart]);
      lookahead--;
    }
    strstart++;
    if (flush)
      FLUSH_BLOCK(0), block_start = strstart;

    /*
     * Make sure that we always have enough lookahead, except at the end of
     * the input file. We need MAX_MATCH bytes for the next match, plus
     * MIN_MATCH bytes to insert the string following the next match.
     */
    while (lookahead < MIN_LOOKAHEAD && !eofile)
      fill_window();

  }
  return FLUSH_BLOCK(1);	  /* eof */
}

#else				/* LAZY */

/*
 * ===========================================================================
 * Same as above, but achieves better compression. We use a lazy evaluation
 * for matches: a match is finally adopted only if there is no better match
 * at the next window position.
 */
ulg deflate()
{
  IPos hash_head;		  /* head of hash chain */
  IPos prev_match;		  /* previous match */
  int flush;			  /* set if current block must be flushed */
  int match_available = 0;	  /* set if previous match exists */
  register unsigned match_length = MIN_MATCH - 1;	/* length of best match */

  /* Process the input block. */
  while (lookahead != 0)
  {

    /*
     * Insert the string window[strstart .. strstart+2] in the dictionary,
     * and set hash_head to the head of the hash chain:
     */
    INSERT_STRING(strstart, hash_head);

    /*
     * Find the longest match, discarding those <= prev_length.
     */
    prev_length = match_length, prev_match = match_start;
    match_length = MIN_MATCH - 1;

    if (hash_head != NIL && prev_length < max_lazy_match &&
	strstart - hash_head <= MAX_DIST)
    {

      /*
       * To simplify the code, we prevent matches with the string of window
       * index 0 (in particular we have to avoid a match of the string with
       * itself at the start of the input file).
       */
      match_length = longest_match(hash_head);
      /* longest_match() sets match_start */
      if (match_length > lookahead)
	match_length = lookahead;

      /* Ignore a length 3 match if it is too distant: */
      if (match_length == MIN_MATCH && strstart - match_start > TOO_FAR)
      {

	/*
	 * If prev_match is also MIN_MATCH, match_start is garbage but we
	 * will ignore the current match anyway.
	 */
	match_length--;
      }
    }

    /*
     * If there was a match at the previous step and the current match is not
     * better, output the previous match:
     */
    if (prev_length >= MIN_MATCH && match_length <= prev_length)
    {

      check_match(strstart - 1, prev_match, prev_length);

      flush = ct_tally(strstart - 1 - prev_match, prev_length - MIN_MATCH);

      /*
       * Insert in hash table all strings up to the end of the match.
       * strstart-1 and strstart are already inserted.
       */
      lookahead -= prev_length - 1;
      prev_length -= 2;
      do
      {
	strstart++;
	INSERT_STRING(strstart, hash_head);

	/*
	 * strstart never exceeds WSIZE-MAX_MATCH, so there are always
	 * MIN_MATCH bytes ahead. If lookahead < MIN_MATCH these bytes are
	 * garbage, but it does not matter since the next lookahead bytes
	 * will always be emitted as literals.
	 */
      } while (--prev_length != 0);
      match_available = 0;
      match_length = MIN_MATCH - 1;
      strstart++;
      if (flush)
	FLUSH_BLOCK(0), block_start = strstart;

    }
    else if (match_available)
    {

      /*
       * If there was no match at the previous position, output a single
       * literal. If there was a match but the current match is longer,
       * truncate the previous match to a single literal.
       */
      Tracevv((stderr, "%c", window[strstart - 1]));
      if (ct_tally(0, window[strstart - 1]))
      {
	FLUSH_BLOCK(0), block_start = strstart;
      }
      strstart++;
      lookahead--;
    }
    else
    {

      /*
       * There is no previous match to compare with, wait for the next step
       * to decide.
       */
      match_available = 1;
      strstart++;
      lookahead--;
    }
    Assert(strstart <= isize && lookahead <= isize, "a bit too far");

    /*
     * Make sure that we always have enough lookahead, except at the end of
     * the input file. We need MAX_MATCH bytes for the next match, plus
     * MIN_MATCH bytes to insert the string following the next match.
     */
    while (lookahead < MIN_LOOKAHEAD && !eofile)
      fill_window();
  }
  if (match_available)
    ct_tally(0, window[strstart - 1]);

  return FLUSH_BLOCK(1);	  /* eof */
}

#endif				/* LAZY */


/*
 * trees.c -- output deflated data using Huffman coding Copyright (C)
 * 1992-1993 Jean-loup Gailly This is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License, see
 * the file COPYING.
 */

/*
 * PURPOSE
 * 
 * Encode various sets of source values using variable-length binary code trees.
 * 
 * DISCUSSION
 * 
 * The PKZIP "deflation" process uses several Huffman trees. The more common
 * source values are represented by shorter bit sequences.
 * 
 * Each code tree is stored in the ZIP file in a compressed form which is itself
 * a Huffman encoding of the lengths of all the code strings (in ascending
 * order by source values). The actual code strings are reconstructed from
 * the lengths in the UNZIP process, as described in the "application note"
 * (APPNOTE.TXT) distributed as part of PKWARE's PKZIP program.
 * 
 * REFERENCES
 * 
 * Lynch, Thomas J. Data Compression:  Techniques and Applications, pp. 53-55.
 * Lifetime Learning Publications, 1985.  ISBN 0-534-03418-7.
 * 
 * Storer, James A. Data Compression:  Methods and Theory, pp. 49-50. Computer
 * Science Press, 1988.  ISBN 0-7167-8156-5.
 * 
 * Sedgewick, R. Algorithms, p290. Addison-Wesley, 1983. ISBN 0-201-06672-6.
 * 
 * INTERFACE
 * 
 * void ct_init (ush *attr, int *methodp) Allocate the match buffer, initialize
 * the various tables and save the location of the internal file attribute
 * (ascii/binary) and method (DEFLATE/STORE)
 * 
 * void ct_tally (int dist, int lc); Save the match info and tally the frequency
 * counts.
 * 
 * long flush_block (char *buf, ulg stored_len, int eof) Determine the best
 * encoding for the current block: dynamic trees, static trees or store, and
 * output the encoded block to the zip file. Returns the total compressed
 * length for the file so far.
 * 
 * static char rcsid[] = "$Id: gzip.c,v 1.2 1994/11/15 14:40:06 sev Exp $";
 */

/*
 * ===========================================================================
 * Constants
 */

#define MAX_BITS 15
/* All codes must not exceed MAX_BITS bits */

#define MAX_BL_BITS 7
/* Bit length codes must not exceed MAX_BL_BITS bits */

#define LENGTH_CODES 29
/* number of length codes, not counting the special END_BLOCK code */

#define LITERALS  256
/* number of literal bytes 0..255 */

#define END_BLOCK 256
/* end of block literal code */

#define L_CODES (LITERALS+1+LENGTH_CODES)
/* number of Literal or Length codes, including the END_BLOCK code */

#define D_CODES   30
/* number of distance codes */

#define BL_CODES  19
/* number of codes used to transfer the bit lengths */


local int near extra_lbits[LENGTH_CODES]	/* extra bits for each length
						 * code */
=
{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};

local int near extra_dbits[D_CODES]	/* extra bits for each distance code */
=
{0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};

local int near extra_blbits[BL_CODES]	/* extra bits for each bit length
					 * code */
=
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 7};

#define STORED_BLOCK 0
#define STATIC_TREES 1
#define DYN_TREES    2
/* The three kinds of block type */

#ifndef LIT_BUFSIZE
#ifdef SMALL_MEM
#define LIT_BUFSIZE  0x2000
#else
#ifdef MEDIUM_MEM
#define LIT_BUFSIZE  0x4000
#else
#define LIT_BUFSIZE  0x8000
#endif
#endif
#endif
#ifndef DIST_BUFSIZE
#define DIST_BUFSIZE  LIT_BUFSIZE
#endif

/*
 * Sizes of match buffers for literals/lengths and distances.  There are 4
 * reasons for limiting LIT_BUFSIZE to 64K: - frequencies can be kept in 16
 * bit counters - if compression is not successful for the first block, all
 * input data is still in the window so we can still emit a stored block even
 * when input comes from standard input.  (This can also be done for all
 * blocks if LIT_BUFSIZE is not greater than 32K.) - if compression is not
 * successful for a file smaller than 64K, we can even emit a stored file
 * instead of a stored block (saving 5 bytes). - creating new Huffman trees
 * less frequently may not provide fast adaptation to changes in the input
 * data statistics. (Take for example a binary file with poorly compressible
 * code followed by a highly compressible string table.) Smaller buffer sizes
 * give fast adaptation but have of course the overhead of transmitting trees
 * more frequently. - I can't count above 4 The current code is general and
 * allows DIST_BUFSIZE < LIT_BUFSIZE (to save memory at the expense of
 * compression). Some optimizations would be possible if we rely on
 * DIST_BUFSIZE == LIT_BUFSIZE.
 */
#if LIT_BUFSIZE > INBUFSIZ
error cannot overlay l_buf and inbuf
#endif

#define REP_3_6      16
/* repeat previous bit length 3-6 times (2 bits of repeat count) */

#define REPZ_3_10    17
/* repeat a zero length 3-10 times  (3 bits of repeat count) */

#define REPZ_11_138  18
/* repeat a zero length 11-138 times  (7 bits of repeat count) */

/*
 * ===========================================================================
 * Local data
 */

/* Data structure describing a single value and its code string. */
typedef struct ct_data
{
  union
  {
    ush freq;			  /* frequency count */
    ush code;			  /* bit string */
  } fc;
  union
  {
    ush dad;			  /* father node in Huffman tree */
    ush len;			  /* length of bit string */
  } dl;
} ct_data;

#define Freq fc.freq
#define Code fc.code
#define Dad  dl.dad
#define Len  dl.len

#define HEAP_SIZE (2*L_CODES+1)
/* maximum heap size */

local ct_data near dyn_ltree[HEAP_SIZE];	/* literal and length tree */
local ct_data near dyn_dtree[2 * D_CODES + 1];	/* distance tree */

local ct_data near static_ltree[L_CODES + 2];

/*
 * The static literal tree. Since the bit lengths are imposed, there is no
 * need for the L_CODES extra codes used during heap construction. However
 * The codes 286 and 287 are needed to build a canonical tree (see ct_init
 * below).
 */

local ct_data near static_dtree[D_CODES];

/*
 * The static distance tree. (Actually a trivial tree since all codes use 5
 * bits.)
 */

local ct_data near bl_tree[2 * BL_CODES + 1];

/* Huffman tree for the bit lengths */

typedef struct tree_desc
{
  ct_data near *dyn_tree;	  /* the dynamic tree */
  ct_data near *static_tree;	  /* corresponding static tree or NULL */
  int near *extra_bits;		  /* extra bits for each code or NULL */
  int extra_base;		  /* base index for extra_bits */
  int elems;			  /* max number of elements in the tree */
  int max_length;		  /* max bit length for the codes */
  int max_code;			  /* largest code with non zero frequency */
} tree_desc;

local tree_desc near l_desc =
{dyn_ltree, static_ltree, extra_lbits, LITERALS + 1, L_CODES, MAX_BITS, 0};

local tree_desc near d_desc =
{dyn_dtree, static_dtree, extra_dbits, 0, D_CODES, MAX_BITS, 0};

local tree_desc near bl_desc =
{bl_tree, (ct_data near *) 0, extra_blbits, 0, BL_CODES, MAX_BL_BITS, 0};


local ush near bl_count[MAX_BITS + 1];

/* number of codes at each bit length for an optimal tree */

local uch near bl_order[BL_CODES]
=
{16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

/*
 * The lengths of the bit length codes are sent in order of decreasing
 * probability, to avoid transmitting the lengths for unused bit length
 * codes.
 */

local int near heap[2 * L_CODES + 1];	/* heap used to build the Huffman
					 * trees */
local int heap_len;		  /* number of elements in the heap */
local int heap_max;		  /* element of largest frequency */

/*
 * The sons of heap[n] are heap[2*n] and heap[2*n+1]. heap[0] is not used.
 * The same heap array is used to build all trees.
 */

local uch near depth[2 * L_CODES + 1];

/* Depth of each subtree used as tie breaker for trees of equal frequency */

local uch length_code[MAX_MATCH - MIN_MATCH + 1];

/* length code for each normalized match length (0 == MIN_MATCH) */

local uch dist_code[512];

/*
 * distance codes. The first 256 values correspond to the distances 3 .. 258,
 * the last 256 values correspond to the top 8 bits of the 15 bit distances.
 */

local int near base_length[LENGTH_CODES];

/* First normalized length for each code (0 = MIN_MATCH) */

local int near base_dist[D_CODES];

/* First normalized distance for each code (0 = distance of 1) */

#define l_buf inbuf
/* DECLARE(uch, l_buf, LIT_BUFSIZE);  buffer for literals or lengths */

/* DECLARE(ush, d_buf, DIST_BUFSIZE); buffer for distances */

local uch near flag_buf[(LIT_BUFSIZE / 8)];

/*
 * flag_buf is a bit array distinguishing literals from lengths in l_buf,
 * thus indicating the presence or absence of a distance.
 */

local unsigned last_lit;	  /* running index in l_buf */
local unsigned last_dist;	  /* running index in d_buf */
local unsigned last_flags;	  /* running index in flag_buf */
local uch flags;		  /* current flags not yet saved in flag_buf */
local uch flag_bit;		  /* current bit used in flags */

/*
 * bits are filled in flags starting at bit 0 (least significant). Note:
 * these flags are overkill in the current code since we don't take advantage
 * of DIST_BUFSIZE == LIT_BUFSIZE.
 */

local ulg opt_len;		  /* bit length of current block with optimal
				   * trees */
local ulg static_len;		  /* bit length of current block with static
				   * trees */

local ulg compressed_len;	  /* total bit length of compressed file */

local ulg input_len;		  /* total byte length of input file */

/* input_len is for debugging only since we can get it by other means. */

ush *file_type;			  /* pointer to UNKNOWN, BINARY or ASCII */
int *file_method;		  /* pointer to DEFLATE or STORE */

extern long block_start;	  /* window offset of current block */
extern unsigned near strstart;	  /* window offset of current string */

/*
 * ===========================================================================
 * Local (static) routines in this file.
 */

local void init_block OF((void));
local void pqdownheap OF((ct_data near * tree, int k));
local void gen_bitlen OF((tree_desc near * desc));
local void gen_codes OF((ct_data near * tree, int max_code));
local void build_tree OF((tree_desc near * desc));
local void scan_tree OF((ct_data near * tree, int max_code));
local void send_tree OF((ct_data near * tree, int max_code));
local int build_bl_tree OF((void));
local void send_all_trees OF((int lcodes, int dcodes, int blcodes));
local void compress_block OF((ct_data near * ltree, ct_data near * dtree));
local void set_file_type OF((void));


#define send_code(c, tree) send_bits(tree[c].Code, tree[c].Len)
/* Send a code of the given tree. c and tree must not have side effects */

#define d_code(dist) \
   ((dist) < 256 ? dist_code[dist] : dist_code[256+((dist)>>7)])

/*
 * Mapping from a distance to a distance code. dist is the distance - 1 and
 * must not have side effects. dist_code[256] and dist_code[257] are never
 * used.
 */

#define MAX(a,b) (a >= b ? a : b)
/* the arguments must not have side effects */

/*
 * ===========================================================================
 * Allocate the match buffer, initialize the various tables and save the
 * location of the internal file attribute (ascii/binary) and method
 * (DEFLATE/STORE).
 */
void ct_init(attr, methodp)
ush *attr;			  /* pointer to internal file attribute */
int *methodp;			  /* pointer to compression method */
{
  int n;			  /* iterates over tree elements */
  int bits;			  /* bit counter */
  int length;			  /* length value */
  int code;			  /* code value */
  int dist;			  /* distance index */

  file_type = attr;
  file_method = methodp;
  compressed_len = input_len = 0L;

  if (static_dtree[0].Len != 0)
    return;			  /* ct_init already called */

  /* Initialize the mapping length (0..255) -> length code (0..28) */
  length = 0;
  for (code = 0; code < LENGTH_CODES - 1; code++)
  {
    base_length[code] = length;
    for (n = 0; n < (1 << extra_lbits[code]); n++)
    {
      length_code[length++] = (uch) code;
    }
  }
  Assert(length == 256, "ct_init: length != 256");

  /*
   * Note that the length 255 (match length 258) can be represented in two
   * different ways: code 284 + 5 bits or code 285, so we overwrite
   * length_code[255] to use the best encoding:
   */
  length_code[length - 1] = (uch) code;

  /* Initialize the mapping dist (0..32K) -> dist code (0..29) */
  dist = 0;
  for (code = 0; code < 16; code++)
  {
    base_dist[code] = dist;
    for (n = 0; n < (1 << extra_dbits[code]); n++)
    {
      dist_code[dist++] = (uch) code;
    }
  }
  Assert(dist == 256, "ct_init: dist != 256");
  dist >>= 7;			  /* from now on, all distances are divided
				   * by 128 */
  for (; code < D_CODES; code++)
  {
    base_dist[code] = dist << 7;
    for (n = 0; n < (1 << (extra_dbits[code] - 7)); n++)
    {
      dist_code[256 + dist++] = (uch) code;
    }
  }
  Assert(dist == 256, "ct_init: 256+dist != 512");

  /* Construct the codes of the static literal tree */
  for (bits = 0; bits <= MAX_BITS; bits++)
    bl_count[bits] = 0;
  n = 0;
  while (n <= 143)
    static_ltree[n++].Len = 8, bl_count[8]++;
  while (n <= 255)
    static_ltree[n++].Len = 9, bl_count[9]++;
  while (n <= 279)
    static_ltree[n++].Len = 7, bl_count[7]++;
  while (n <= 287)
    static_ltree[n++].Len = 8, bl_count[8]++;

  /*
   * Codes 286 and 287 do not exist, but we must include them in the tree
   * construction to get a canonical Huffman tree (longest code all ones)
   */
  gen_codes((ct_data near *) static_ltree, L_CODES + 1);

  /* The static distance tree is trivial: */
  for (n = 0; n < D_CODES; n++)
  {
    static_dtree[n].Len = 5;
    static_dtree[n].Code = bi_reverse(n, 5);
  }

  /* Initialize the first block of the first file: */
  init_block();
}

/*
 * ===========================================================================
 * Initialize a new block.
 */
local void init_block()
{
  int n;			  /* iterates over tree elements */

  /* Initialize the trees. */
  for (n = 0; n < L_CODES; n++)
    dyn_ltree[n].Freq = 0;
  for (n = 0; n < D_CODES; n++)
    dyn_dtree[n].Freq = 0;
  for (n = 0; n < BL_CODES; n++)
    bl_tree[n].Freq = 0;

  dyn_ltree[END_BLOCK].Freq = 1;
  opt_len = static_len = 0L;
  last_lit = last_dist = last_flags = 0;
  flags = 0;
  flag_bit = 1;
}

#define SMALLEST 1
/* Index within the heap array of least frequent node in the Huffman tree */


/*
 * ===========================================================================
 * Remove the smallest element from the heap and recreate the heap with one
 * less element. Updates heap and heap_len.
 */
#define pqremove(tree, top) \
{\
    top = heap[SMALLEST]; \
    heap[SMALLEST] = heap[heap_len--]; \
    pqdownheap(tree, SMALLEST); \
}

/*
 * ===========================================================================
 * Compares to subtrees, using the tree depth as tie breaker when the
 * subtrees have equal frequency. This minimizes the worst case length.
 */
#define smaller(tree, n, m) \
   (tree[n].Freq < tree[m].Freq || \
   (tree[n].Freq == tree[m].Freq && depth[n] <= depth[m]))

/*
 * ===========================================================================
 * Restore the heap property by moving down the tree starting at node k,
 * exchanging a node with the smallest of its two sons if necessary, stopping
 * when the heap property is re-established (each father smaller than its two
 * sons).
 */
local void pqdownheap(tree, k)
ct_data near *tree;		  /* the tree to restore */
int k;				  /* node to move down */
{
  int v = heap[k];
  int j = k << 1;		  /* left son of k */

  while (j <= heap_len)
  {
    /* Set j to the smallest of the two sons: */
    if (j < heap_len && smaller(tree, heap[j + 1], heap[j]))
      j++;

    /* Exit if v is smaller than both sons */
    if (smaller(tree, v, heap[j]))
      break;

    /* Exchange v with the smallest son */
    heap[k] = heap[j];
    k = j;

    /* And continue down the tree, setting j to the left son of k */
    j <<= 1;
  }
  heap[k] = v;
}

/*
 * ===========================================================================
 * Compute the optimal bit lengths for a tree and update the total bit length
 * for the current block. IN assertion: the fields freq and dad are set,
 * heap[heap_max] and above are the tree nodes sorted by increasing
 * frequency. OUT assertions: the field len is set to the optimal bit length,
 * the array bl_count contains the frequencies for each bit length. The
 * length opt_len is updated; static_len is also updated if stree is not
 * null.
 */
local void gen_bitlen(desc)
tree_desc near *desc;		  /* the tree descriptor */
{
  ct_data near *tree = desc->dyn_tree;
  int near *extra = desc->extra_bits;
  int base = desc->extra_base;
  int max_code = desc->max_code;
  int max_length = desc->max_length;
  ct_data near *stree = desc->static_tree;
  int h;			  /* heap index */
  int n, m;			  /* iterate over the tree elements */
  int bits;			  /* bit length */
  int xbits;			  /* extra bits */
  ush f;			  /* frequency */
  int overflow = 0;		  /* number of elements with bit length too
				   * large */

  for (bits = 0; bits <= MAX_BITS; bits++)
    bl_count[bits] = 0;

  /*
   * In a first pass, compute the optimal bit lengths (which may overflow in
   * the case of the bit length tree).
   */
  tree[heap[heap_max]].Len = 0;	  /* root of the heap */

  for (h = heap_max + 1; h < HEAP_SIZE; h++)
  {
    n = heap[h];
    bits = tree[tree[n].Dad].Len + 1;
    if (bits > max_length)
      bits = max_length, overflow++;
    tree[n].Len = (ush) bits;
    /* We overwrite tree[n].Dad which is no longer needed */

    if (n > max_code)
      continue;			  /* not a leaf node */

    bl_count[bits]++;
    xbits = 0;
    if (n >= base)
      xbits = extra[n - base];
    f = tree[n].Freq;
    opt_len += (ulg) f *(bits + xbits);

    if (stree)
      static_len += (ulg) f *(stree[n].Len + xbits);
  }
  if (overflow == 0)
    return;

  Trace((stderr, "\nbit length overflow\n"));
  /* This happens for example on obj2 and pic of the Calgary corpus */

  /* Find the first bit length which could increase: */
  do
  {
    bits = max_length - 1;
    while (bl_count[bits] == 0)
      bits--;
    bl_count[bits]--;		  /* move one leaf down the tree */
    bl_count[bits + 1] += 2;	  /* move one overflow item as its brother */
    bl_count[max_length]--;

    /*
     * The brother of the overflow item also moves one step up, but this does
     * not affect bl_count[max_length]
     */
    overflow -= 2;
  } while (overflow > 0);

  /*
   * Now recompute all bit lengths, scanning in increasing frequency. h is
   * still equal to HEAP_SIZE. (It is simpler to reconstruct all lengths
   * instead of fixing only the wrong ones. This idea is taken from 'ar'
   * written by Haruhiko Okumura.)
   */
  for (bits = max_length; bits != 0; bits--)
  {
    n = bl_count[bits];
    while (n != 0)
    {
      m = heap[--h];
      if (m > max_code)
	continue;
      if (tree[m].Len != (unsigned) bits)
      {
	Trace((stderr, "code %d bits %d->%d\n", m, tree[m].Len, bits));
	opt_len += ((long) bits - (long) tree[m].Len) * (long) tree[m].Freq;
	tree[m].Len = (ush) bits;
      }
      n--;
    }
  }
}

/*
 * ===========================================================================
 * Generate the codes for a given tree and bit counts (which need not be
 * optimal). IN assertion: the array bl_count contains the bit length
 * statistics for the given tree and the field len is set for all tree
 * elements. OUT assertion: the field code is set for all tree elements of
 * non zero code length.
 */
local void gen_codes(tree, max_code)
ct_data near *tree;		  /* the tree to decorate */
int max_code;			  /* largest code with non zero frequency */
{
  ush next_code[MAX_BITS + 1];	  /* next code value for each bit length */
  ush code = 0;			  /* running code value */
  int bits;			  /* bit index */
  int n;			  /* code index */

  /*
   * The distribution counts are first used to generate the code values
   * without bit reversal.
   */
  for (bits = 1; bits <= MAX_BITS; bits++)
  {
    next_code[bits] = code = (code + bl_count[bits - 1]) << 1;
  }

  /*
   * Check that the bit counts in bl_count are consistent. The last code must
   * be all ones.
   */
  Assert(code + bl_count[MAX_BITS] - 1 == (1 << MAX_BITS) - 1,
	 "inconsistent bit counts");
  Tracev((stderr, "\ngen_codes: max_code %d ", max_code));

  for (n = 0; n <= max_code; n++)
  {
    int len = tree[n].Len;

    if (len == 0)
      continue;
    /* Now reverse the bits */
    tree[n].Code = bi_reverse(next_code[len]++, len);

    Tracec(tree != static_ltree, (stderr, "\nn %3d %c l %2d c %4x (%x) ",
	 n, (isgraph(n) ? n : ' '), len, tree[n].Code, next_code[len] - 1));
  }
}

/*
 * ===========================================================================
 * Construct one Huffman tree and assigns the code bit strings and lengths.
 * Update the total bit length for the current block. IN assertion: the field
 * freq is set for all tree elements. OUT assertions: the fields len and code
 * are set to the optimal bit length and corresponding code. The length
 * opt_len is updated; static_len is also updated if stree is not null. The
 * field max_code is set.
 */
local void build_tree(desc)
tree_desc near *desc;		  /* the tree descriptor */
{
  ct_data near *tree = desc->dyn_tree;
  ct_data near *stree = desc->static_tree;
  int elems = desc->elems;
  int n, m;			  /* iterate over heap elements */
  int max_code = -1;		  /* largest code with non zero frequency */
  int node = elems;		  /* next internal node of the tree */

  /*
   * Construct the initial heap, with least frequent element in
   * heap[SMALLEST]. The sons of heap[n] are heap[2*n] and heap[2*n+1].
   * heap[0] is not used.
   */
  heap_len = 0, heap_max = HEAP_SIZE;

  for (n = 0; n < elems; n++)
  {
    if (tree[n].Freq != 0)
    {
      heap[++heap_len] = max_code = n;
      depth[n] = 0;
    }
    else
    {
      tree[n].Len = 0;
    }
  }

  /*
   * The pkzip format requires that at least one distance code exists, and
   * that at least one bit should be sent even if there is only one possible
   * code. So to avoid special checks later on we force at least two codes of
   * non zero frequency.
   */
  while (heap_len < 2)
  {
    int new = heap[++heap_len] = (max_code < 2 ? ++max_code : 0);

    tree[new].Freq = 1;
    depth[new] = 0;
    opt_len--;
    if (stree)
      static_len -= stree[new].Len;
    /* new is 0 or 1 so it does not have extra bits */
  }
  desc->max_code = max_code;

  /*
   * The elements heap[heap_len/2+1 .. heap_len] are leaves of the tree,
   * establish sub-heaps of increasing lengths:
   */
  for (n = heap_len / 2; n >= 1; n--)
    pqdownheap(tree, n);

  /*
   * Construct the Huffman tree by repeatedly combining the least two
   * frequent nodes.
   */
  do
  {
    pqremove(tree, n);		  /* n = node of least frequency */
    m = heap[SMALLEST];		  /* m = node of next least frequency */

    heap[--heap_max] = n;	  /* keep the nodes sorted by frequency */
    heap[--heap_max] = m;

    /* Create a new node father of n and m */
    tree[node].Freq = tree[n].Freq + tree[m].Freq;
    depth[node] = (uch) (MAX(depth[n], depth[m]) + 1);
    tree[n].Dad = tree[m].Dad = (ush) node;
    /* and insert the new node in the heap */
    heap[SMALLEST] = node++;
    pqdownheap(tree, SMALLEST);

  } while (heap_len >= 2);

  heap[--heap_max] = heap[SMALLEST];

  /*
   * At this point, the fields freq and dad are set. We can now generate the
   * bit lengths.
   */
  gen_bitlen((tree_desc near *) desc);

  /* The field len is now set, we can generate the bit codes */
  gen_codes((ct_data near *) tree, max_code);
}

/*
 * ===========================================================================
 * Scan a literal or distance tree to determine the frequencies of the codes
 * in the bit length tree. Updates opt_len to take into account the repeat
 * counts. (The contribution of the bit length codes will be added later
 * during the construction of bl_tree.)
 */
local void scan_tree(tree, max_code)
ct_data near *tree;		  /* the tree to be scanned */
int max_code;			  /* and its largest code of non zero
				   * frequency */
{
  int n;			  /* iterates over all tree elements */
  int prevlen = -1;		  /* last emitted length */
  int curlen;			  /* length of current code */
  int nextlen = tree[0].Len;	  /* length of next code */
  int count = 0;		  /* repeat count of the current code */
  int max_count = 7;		  /* max repeat count */
  int min_count = 4;		  /* min repeat count */

  if (nextlen == 0)
    max_count = 138, min_count = 3;
  tree[max_code + 1].Len = (ush) - 1;	/* guard */

  for (n = 0; n <= max_code; n++)
  {
    curlen = nextlen;
    nextlen = tree[n + 1].Len;
    if (++count < max_count && curlen == nextlen)
    {
      continue;
    }
    else if (count < min_count)
    {
      bl_tree[curlen].Freq += count;
    }
    else if (curlen != 0)
    {
      if (curlen != prevlen)
	bl_tree[curlen].Freq++;
      bl_tree[REP_3_6].Freq++;
    }
    else if (count <= 10)
    {
      bl_tree[REPZ_3_10].Freq++;
    }
    else
    {
      bl_tree[REPZ_11_138].Freq++;
    }
    count = 0;
    prevlen = curlen;
    if (nextlen == 0)
    {
      max_count = 138, min_count = 3;
    }
    else if (curlen == nextlen)
    {
      max_count = 6, min_count = 3;
    }
    else
    {
      max_count = 7, min_count = 4;
    }
  }
}

/*
 * ===========================================================================
 * Send a literal or distance tree in compressed form, using the codes in
 * bl_tree.
 */
local void send_tree(tree, max_code)
ct_data near *tree;		  /* the tree to be scanned */
int max_code;			  /* and its largest code of non zero
				   * frequency */
{
  int n;			  /* iterates over all tree elements */
  int prevlen = -1;		  /* last emitted length */
  int curlen;			  /* length of current code */
  int nextlen = tree[0].Len;	  /* length of next code */
  int count = 0;		  /* repeat count of the current code */
  int max_count = 7;		  /* max repeat count */
  int min_count = 4;		  /* min repeat count */

  /* tree[max_code+1].Len = -1; *//* guard already set */
  if (nextlen == 0)
    max_count = 138, min_count = 3;

  for (n = 0; n <= max_code; n++)
  {
    curlen = nextlen;
    nextlen = tree[n + 1].Len;
    if (++count < max_count && curlen == nextlen)
    {
      continue;
    }
    else if (count < min_count)
    {
      do
      {
	send_code(curlen, bl_tree);
      } while (--count != 0);

    }
    else if (curlen != 0)
    {
      if (curlen != prevlen)
      {
	send_code(curlen, bl_tree);
	count--;
      }
      Assert(count >= 3 && count <= 6, " 3_6?");
      send_code(REP_3_6, bl_tree);
      send_bits(count - 3, 2);

    }
    else if (count <= 10)
    {
      send_code(REPZ_3_10, bl_tree);
      send_bits(count - 3, 3);

    }
    else
    {
      send_code(REPZ_11_138, bl_tree);
      send_bits(count - 11, 7);
    }
    count = 0;
    prevlen = curlen;
    if (nextlen == 0)
    {
      max_count = 138, min_count = 3;
    }
    else if (curlen == nextlen)
    {
      max_count = 6, min_count = 3;
    }
    else
    {
      max_count = 7, min_count = 4;
    }
  }
}

/*
 * ===========================================================================
 * Construct the Huffman tree for the bit lengths and return the index in
 * bl_order of the last bit length code to send.
 */
local int build_bl_tree()
{
  int max_blindex;		  /* index of last bit length code of non
				   * zero freq */

  /* Determine the bit length frequencies for literal and distance trees */
  scan_tree((ct_data near *) dyn_ltree, l_desc.max_code);
  scan_tree((ct_data near *) dyn_dtree, d_desc.max_code);

  /* Build the bit length tree: */
  build_tree((tree_desc near *) (&bl_desc));

  /*
   * opt_len now includes the length of the tree representations, except the
   * lengths of the bit lengths codes and the 5+5+4 bits for the counts.
   */

  /*
   * Determine the number of bit length codes to send. The pkzip format
   * requires that at least 4 bit length codes be sent. (appnote.txt says 3
   * but the actual value used is 4.)
   */
  for (max_blindex = BL_CODES - 1; max_blindex >= 3; max_blindex--)
  {
    if (bl_tree[bl_order[max_blindex]].Len != 0)
      break;
  }
  /* Update opt_len to include the bit length tree and counts */
  opt_len += 3 * (max_blindex + 1) + 5 + 5 + 4;
  Tracev((stderr, "\ndyn trees: dyn %ld, stat %ld", opt_len, static_len));

  return max_blindex;
}

/*
 * ===========================================================================
 * Send the header for a block using dynamic Huffman trees: the counts, the
 * lengths of the bit length codes, the literal tree and the distance tree.
 * IN assertion: lcodes >= 257, dcodes >= 1, blcodes >= 4.
 */
local void send_all_trees(lcodes, dcodes, blcodes)
int lcodes, dcodes, blcodes;	  /* number of codes for each tree */
{
  int rank;			  /* index in bl_order */

  Assert(lcodes >= 257 && dcodes >= 1 && blcodes >= 4, "not enough codes");
  Assert(lcodes <= L_CODES && dcodes <= D_CODES && blcodes <= BL_CODES,
	 "too many codes");
  Tracev((stderr, "\nbl counts: "));
  send_bits(lcodes - 257, 5);	  /* not +255 as stated in appnote.txt */
  send_bits(dcodes - 1, 5);
  send_bits(blcodes - 4, 4);	  /* not -3 as stated in appnote.txt */
  for (rank = 0; rank < blcodes; rank++)
  {
    Tracev((stderr, "\nbl code %2d ", bl_order[rank]));
    send_bits(bl_tree[bl_order[rank]].Len, 3);
  }
  Tracev((stderr, "\nbl tree: sent %ld", bits_sent));

  send_tree((ct_data near *) dyn_ltree, lcodes - 1);	/* send the literal tree */
  Tracev((stderr, "\nlit tree: sent %ld", bits_sent));

  send_tree((ct_data near *) dyn_dtree, dcodes - 1);	/* send the distance
							 * tree */
  Tracev((stderr, "\ndist tree: sent %ld", bits_sent));
}

/*
 * ===========================================================================
 * Determine the best encoding for the current block: dynamic trees, static
 * trees or store, and output the encoded block to the zip file. This
 * function returns the total compressed length for the file so far.
 */
ulg flush_block(buf, stored_len, eof)
char *buf;			  /* input block, or NULL if too old */
ulg stored_len;			  /* length of input block */
int eof;			  /* true if this is the last block for a
				   * file */
{
  ulg opt_lenb, static_lenb;	  /* opt_len and static_len in bytes */
  int max_blindex;		  /* index of last bit length code of non
				   * zero freq */

  flag_buf[last_flags] = flags;	  /* Save the flags for the last 8 items */

  /* Check if the file is ascii or binary */
  if (*file_type == (ush) UNKNOWN)
    set_file_type();

  /* Construct the literal and distance trees */
  build_tree((tree_desc near *) (&l_desc));
  Tracev((stderr, "\nlit data: dyn %ld, stat %ld", opt_len, static_len));

  build_tree((tree_desc near *) (&d_desc));
  Tracev((stderr, "\ndist data: dyn %ld, stat %ld", opt_len, static_len));

  /*
   * At this point, opt_len and static_len are the total bit lengths of the
   * compressed block data, excluding the tree representations.
   */

  /*
   * Build the bit length tree for the above two trees, and get the index in
   * bl_order of the last bit length code to send.
   */
  max_blindex = build_bl_tree();

  /* Determine the best encoding. Compute first the block length in bytes */
  opt_lenb = (opt_len + 3 + 7) >> 3;
  static_lenb = (static_len + 3 + 7) >> 3;
  input_len += stored_len;	  /* for debugging only */

  Trace((stderr, "\nopt %lu(%lu) stat %lu(%lu) stored %lu lit %u dist %u ",
	 opt_lenb, opt_len, static_lenb, static_len, stored_len,
	 last_lit, last_dist));

  if (static_lenb <= opt_lenb)
    opt_lenb = static_lenb;

  /*
   * If compression failed and this is the first and last block, and if the
   * zip file can be seeked (to rewrite the local header), the whole file is
   * transformed into a stored file:
   */
#ifdef FORCE_METHOD
  if (level == 1 && eof && compressed_len == 0L)
  {				  /* force stored file */
#else
  if (stored_len <= opt_lenb && eof && compressed_len == 0L && seekable())
  {
#endif
    /* Since LIT_BUFSIZE <= 2*WSIZE, the input data must be there: */
    if (buf == (char *) 0)
      error("block vanished");

    copy_block(buf, (unsigned) stored_len, 0);	/* without header */
    compressed_len = stored_len << 3;
    *file_method = STORED;

#ifdef FORCE_METHOD
  }
  else if (level == 2 && buf != (char *) 0)
  {				  /* force stored block */
#else
  }
  else if (stored_len + 4 <= opt_lenb && buf != (char *) 0)
  {
    /* 4: two words for the lengths */
#endif

    /*
     * The test buf != NULL is only necessary if LIT_BUFSIZE > WSIZE.
     * Otherwise we can't have processed more than WSIZE input bytes since
     * the last block flush, because compression would have been successful.
     * If LIT_BUFSIZE <= WSIZE, it is never too late to transform a block
     * into a stored block.
     */
    send_bits((STORED_BLOCK << 1) + eof, 3);	/* send block type */
    compressed_len = (compressed_len + 3 + 7) & ~7L;
    compressed_len += (stored_len + 4) << 3;

    copy_block(buf, (unsigned) stored_len, 1);	/* with header */

#ifdef FORCE_METHOD
  }
  else if (level == 3)
  {				  /* force static trees */
#else
  }
  else if (static_lenb == opt_lenb)
  {
#endif
    send_bits((STATIC_TREES << 1) + eof, 3);
    compress_block((ct_data near *) static_ltree, (ct_data near *) static_dtree);
    compressed_len += 3 + static_len;
  }
  else
  {
    send_bits((DYN_TREES << 1) + eof, 3);
    send_all_trees(l_desc.max_code + 1, d_desc.max_code + 1, max_blindex + 1);
    compress_block((ct_data near *) dyn_ltree, (ct_data near *) dyn_dtree);
    compressed_len += 3 + opt_len;
  }
  Assert(compressed_len == bits_sent, "bad compressed size");
  init_block();

  if (eof)
  {
    Assert(input_len == isize, "bad input size");
    bi_windup();
    compressed_len += 7;	  /* align on byte boundary */
  }
  Tracev((stderr, "\ncomprlen %lu(%lu) ", compressed_len >> 3,
	  compressed_len - 7 * eof));

  return compressed_len >> 3;
}

/*
 * ===========================================================================
 * Save the match info and tally the frequency counts. Return true if the
 * current block must be flushed.
 */
int ct_tally(dist, lc)
int dist;			  /* distance of matched string */
int lc;				  /* match length-MIN_MATCH or unmatched char
				   * (if dist==0) */
{
  l_buf[last_lit++] = (uch) lc;
  if (dist == 0)
  {
    /* lc is the unmatched char */
    dyn_ltree[lc].Freq++;
  }
  else
  {
    /* Here, lc is the match length - MIN_MATCH */
    dist--;			  /* dist = match distance - 1 */
    Assert((ush) dist < (ush) MAX_DIST &&
	   (ush) lc <= (ush) (MAX_MATCH - MIN_MATCH) &&
	   (ush) d_code(dist) < (ush) D_CODES, "ct_tally: bad match");

    dyn_ltree[length_code[lc] + LITERALS + 1].Freq++;
    dyn_dtree[d_code(dist)].Freq++;

    d_buf[last_dist++] = (ush) dist;
    flags |= flag_bit;
  }
  flag_bit <<= 1;

  /* Output the flags if they fill a byte: */
  if ((last_lit & 7) == 0)
  {
    flag_buf[last_flags++] = flags;
    flags = 0, flag_bit = 1;
  }
  /* Try to guess if it is profitable to stop the current block here */
  if (level > 2 && (last_lit & 0xfff) == 0)
  {
    /* Compute an upper bound for the compressed length */
    ulg out_length = (ulg) last_lit * 8L;
    ulg in_length = (ulg) strstart - block_start;
    int dcode;

    for (dcode = 0; dcode < D_CODES; dcode++)
    {
      out_length += (ulg) dyn_dtree[dcode].Freq * (5L + extra_dbits[dcode]);
    }
    out_length >>= 3;
    Trace((stderr, "\nlast_lit %u, last_dist %u, in %ld, out ~%ld(%ld%%) ",
	   last_lit, last_dist, in_length, out_length,
	   100L - out_length * 100L / in_length));
    if (last_dist < last_lit / 2 && out_length < in_length / 2)
      return 1;
  }
  return (last_lit == LIT_BUFSIZE - 1 || last_dist == DIST_BUFSIZE);

  /*
   * We avoid equality with LIT_BUFSIZE because of wraparound at 64K on 16
   * bit machines and because stored blocks are restricted to 64K-1 bytes.
   */
}

/*
 * ===========================================================================
 * Send the block data compressed using the given Huffman trees
 */
local void compress_block(ltree, dtree)
ct_data near *ltree;		  /* literal tree */
ct_data near *dtree;		  /* distance tree */
{
  unsigned dist;		  /* distance of matched string */
  int lc;			  /* match length or unmatched char (if dist
				   * == 0) */
  unsigned lx = 0;		  /* running index in l_buf */
  unsigned dx = 0;		  /* running index in d_buf */
  unsigned fx = 0;		  /* running index in flag_buf */
  uch flag = 0;			  /* current flags */
  unsigned code;		  /* the code to send */
  int extra;			  /* number of extra bits to send */

  if (last_lit != 0)
    do
    {
      if ((lx & 7) == 0)
	flag = flag_buf[fx++];
      lc = l_buf[lx++];
      if ((flag & 1) == 0)
      {
	send_code(lc, ltree);	  /* send a literal byte */
	Tracecv(isgraph(lc), (stderr, " '%c' ", lc));
      }
      else
      {
	/* Here, lc is the match length - MIN_MATCH */
	code = length_code[lc];
	send_code(code + LITERALS + 1, ltree);	/* send the length code */
	extra = extra_lbits[code];
	if (extra != 0)
	{
	  lc -= base_length[code];
	  send_bits(lc, extra);	  /* send the extra length bits */
	}
	dist = d_buf[dx++];
	/* Here, dist is the match distance - 1 */
	code = d_code(dist);
	Assert(code < D_CODES, "bad d_code");

	send_code(code, dtree);	  /* send the distance code */
	extra = extra_dbits[code];
	if (extra != 0)
	{
	  dist -= base_dist[code];
	  send_bits(dist, extra); /* send the extra distance bits */
	}
      }				  /* literal or match pair ? */
      flag >>= 1;
    } while (lx < last_lit);

  send_code(END_BLOCK, ltree);
}

/*
 * ===========================================================================
 * Set the file type to ASCII or BINARY, using a crude approximation: binary
 * if more than 20% of the bytes are <= 6 or >= 128, ascii otherwise. IN
 * assertion: the fields freq of dyn_ltree are set and the total of all
 * frequencies does not exceed 64K (to fit in an int on 16 bit machines).
 */
local void set_file_type()
{
  int n = 0;
  unsigned ascii_freq = 0;
  unsigned bin_freq = 0;

  while (n < 7)
    bin_freq += dyn_ltree[n++].Freq;
  while (n < 128)
    ascii_freq += dyn_ltree[n++].Freq;
  while (n < LITERALS)
    bin_freq += dyn_ltree[n++].Freq;
  *file_type = bin_freq > (ascii_freq >> 2) ? BINARY : ASCII;
  if (*file_type == BINARY && translate_eol)
  {
    warn("-l used on binary file", "");
  }
}


/*
 * util.c -- utility functions for gzip support Copyright (C) 1992-1993
 * Jean-loup Gailly This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License, see the file
 * COPYING.
 * 
 * static char rcsid[] = "$Id: gzip.c,v 1.2 1994/11/15 14:40:06 sev Exp $";
 */

extern ulg crc_32_tab[];	  /* crc table, defined below */

/*
 * ===========================================================================
 * Run a set of bytes through the crc shift register.  If s is a NULL
 * pointer, then initialize the crc shift register contents instead. Return
 * the current crc in either case.
 */
ulg updcrc(s, n)
uch *s;				  /* pointer to bytes to pump through */
unsigned n;			  /* number of bytes in s[] */
{
  register ulg c;		  /* temporary variable */

  static ulg crc = (ulg) 0xffffffffL;	/* shift register contents */

  if (s == NULL)
  {
    c = 0xffffffffL;
  }
  else
  {
    c = crc;
    while (n--)
    {
      c = crc_32_tab[((int) c ^ (*s++)) & 0xff] ^ (c >> 8);
    }
  }
  crc = c;
  return c ^ 0xffffffffL;	  /* (instead of ~c for 64-bit machines) */
}

/*
 * ===========================================================================
 * Clear input and output buffers
 */
void clear_bufs()
{
  outcnt = 0;
  insize = inptr = 0;
  bytes_in = bytes_out = 0L;
}

/*
 * ===========================================================================
 * Write the output buffer outbuf[0..outcnt-1] and update bytes_out. (used
 * for the compressed data only)
 */
void flush_outbuf()
{
  if (outcnt == 0)
    return;

  if (write(ofd, (char *) outbuf, outcnt) != outcnt)
  {
    write_error();
  }
  bytes_out += (ulg) outcnt;
  outcnt = 0;
}

/*
 * ========================================================================
 * Return the base name of a file (remove any directory prefix).
 */
char *basename(fname)
char *fname;
{
  char *p;

  if ((p = strrchr(fname, PATH_SEP)) != NULL)
    fname = p + 1;
#ifdef OTHER_PATH_SEP
  if ((p = strrchr(fname, OTHER_PATH_SEP)) != NULL)
    fname = p + 1;
#endif
  return fname;
}

/*
 * ========================================================================
 * Error handlers.
 */
void error(m)
char *m;
{
  fprintf(stderr, "%s: %s\n", ifname, m);
  abort_gzip();
}

void warn(a, b)
char *a, *b;			  /* message strings juxtaposed in output */
{
  fprintf(stderr, " warning: %s%s\n", a, b);
  if (exit_code == OK)
    exit_code = WARNING;
}

void write_error()
{
  fprintf(stderr, "\nwrite error on ");
  perror(ofname);
  abort_gzip();
}

/*
 * ========================================================================
 * Table of CRC-32's of all single-byte values (made by makecrc.c)
 */
ulg crc_32_tab[] =
{
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};


/*
 * zip.c -- compress files to the gzip or pkzip format Copyright (C)
 * 1992-1993 Jean-loup Gailly This is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License, see
 * the file COPYING.
 * 
 * static char rcsid[] = "$Id: gzip.c,v 1.2 1994/11/15 14:40:06 sev Exp $";
 */

local ulg crc;			  /* crc on uncompressed file data */
long overhead;			  /* number of bytes in gzip header */

/*
 * ===========================================================================
 * Deflate in to out. IN assertions: the input and output buffers are
 * cleared.
 */
void zip(in, out)
int in, out;			  /* input and output file descriptors */
{
  uch flags = 0;		  /* general purpose bit flags */
  ush attr = 0;			  /* ascii/binary flag */
  ush deflate_flags = 0;	  /* pkzip -es, -en or -ex equivalent */

  ifd = in;

  ofd = out;
  outcnt = 0;

  /* Write the header to the gzip file. See algorithm.doc for the format */

  method = DEFLATED;
  put_byte(GZIP_MAGIC[0]);	  /* magic header */
  put_byte(GZIP_MAGIC[1]);
  put_byte(DEFLATED);		  /* compression method */

  if (save_orig_name)
  {
    flags |= ORIG_NAME;
  }

  /*
   * Should look ahead a little in the input to determine the ascii/binary
   * flag. To be done.
   */

  put_byte(flags);		  /* general flags */
  put_long(time_stamp);

  /* Write deflated file to zip file */
  crc = updcrc(0, 0);

  bi_init(out);
  ct_init(&attr, &method);
  lm_init(level, &deflate_flags);

  put_byte((uch) deflate_flags);  /* extra flags */
  put_byte(OS_CODE);		  /* OS identifier */

  if (save_orig_name)
  {
    char *p = basename(ifname);	  /* Don't save the directory part. */

    do
    {
      put_byte(casemap(*p));
    } while (*p++);
  }
  overhead = (long) outcnt;

  (void) deflate();

#if !defined(NO_SIZE_CHECK) && !defined(RECORD_IO)

  /*
   * Check input size (but not in VMS -- variable record lengths mess it up)
   * and not on MSDOS -- diet in TSR mode reports an incorrect file size)
   */
  if (ifile_size != -1L && isize != (ulg) ifile_size)
  {
    Trace((stderr, " actual=%ld, read=%ld ", ifile_size, isize));
    fprintf(stderr, " file size changed while zipping %s\n", ifname);
  }
#endif

  /* Write the crc and uncompressed size */
  put_long(crc);
  put_long(isize);

  flush_outbuf();
}


/*
 * ===========================================================================
 * Read a new buffer from the current input file, perform end-of-line
 * translation, and update the crc and input file size. IN assertion: size >=
 * 2 (for end-of-line translation)
 */
int file_read(buf, size)
char *buf;
unsigned size;
{
  unsigned len;

  Assert(insize == 0, "inbuf not empty");

  len = read(ifd, buf, size);
  if (len == (unsigned) (-1) || len == 0)
    return (int) len;

  crc = updcrc((uch *) buf, len);
  isize += (ulg) len;
  return (int) len;
}
