/* gentr46map.c - generate TR46 lookup tables
   Copyright (C) 2016 Tim Ruehsen

   Libidn2 is free software: you can redistribute it and/or modify it
   under the terms of either:

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at
       your option) any later version.

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at
       your option) any later version.

   or both in parallel, as here.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see <http://www.gnu.org/licenses/>.
*/

#include <config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#include <unistr.h>

#include "tr46map.h"

#define countof(a) (sizeof(a)/sizeof(*(a)))

static IDNAMap idna_map[10000];
static size_t map_pos;

static uint8_t genmapdata[16384];
static size_t mapdata_pos;

static NFCQCMap nfcqc_map[140];
static size_t nfcqc_pos;

static char *
_nextField (char **line)
{
  char *s = *line, *e;

  if (!*s)
    return NULL;

  if (!(e = strpbrk (s, ";#")))
    {
      e = *line += strlen (s);
    }
  else
    {
      *line = e + (*e == ';');
      *e = 0;
    }

  // trim leading and trailing whitespace
  while (isspace (*s))
    s++;
  while (e > s && isspace (e[-1]))
    *--e = 0;

  return s;
}

static int
_scan_file (const char *fname, int (*scan) (char *))
{
  FILE *fp = fopen (fname, "r");
  char *buf = NULL, *linep;
  size_t bufsize = 0;
  ssize_t buflen;
  int ret = 0;

  if (!fp)
    {
      fprintf (stderr, "Failed to open %s (%d)\n", fname, errno);
      return -1;
    }

  while ((buflen = getline (&buf, &bufsize, fp)) >= 0)
    {
      linep = buf;

      while (isspace (*linep))
	linep++;		// ignore leading whitespace

      // strip off \r\n
      while (buflen > 0 && (buf[buflen] == '\n' || buf[buflen] == '\r'))
	buf[--buflen] = 0;

      if (!*linep || *linep == '#')
	continue;		// skip empty lines and comments

      if ((ret = scan (linep)))
	break;
    }

  free (buf);
  fclose (fp);

  return ret;
}

static size_t
_u32_stream_len(uint32_t *src, size_t len)
{
  int it;
  size_t n = 0;

/*
1 byte: 0-0x7f -> 0xxxxxxx
2 bytes: 0x80-0x3fff ->1xxxxxxx 0xxxxxxx
3 bytes: 0x4000-0x1fffff ->1xxxxxxx 1xxxxxxx 0xxxxxxx
4 bytes: 0x200000-0xFFFFFFF -> 1xxxxxxx 1xxxxxxx 1xxxxxxx 0xxxxxxx
5 bytes: 0x10000000->0xFFFFFFFF -> 1xxxxxxx 1xxxxxxx 1xxxxxxx 1xxxxxxx
*/
  for (it = 0; it < len; it++)
    {
      uint32_t cp = src[it];

      if (cp <= 0x7f)
	n += 1;
      else if (cp <= 0x3fff)
	n += 2;
      else if (cp <= 0x1fffff)
	n += 3;
      else if (cp <= 0xFFFFFFF)
	n += 4;
      else
	n += 5;
    }

  return n;
}

static size_t
_u32_to_stream(uint8_t *dst, size_t dst_size, uint32_t *src, size_t len)
{
  int it;
  size_t n = 0;

  n = _u32_stream_len(src, len);

  if (!dst)
    return n;

  if (dst_size < n)
    return 0;

  for (it = 0; it < len; it++)
    {
      uint32_t cp = src[it];

      if (cp <= 0x7f)
	*dst++ = cp & 0x7F;
      else if (cp <= 0x3fff)
	{
	  *dst++ = 0x80 | ((cp >> 7) & 0x7F);
	  *dst++ = cp & 0x7F;
	}
      else if (cp <= 0x1fffff)
        {
	  *dst++ = 0x80 | ((cp >> 14) & 0x7F);
	  *dst++ = 0x80 | ((cp >> 7) & 0x7F);
	  *dst++ = cp & 0x7F;
        }
      else if (cp <= 0xFFFFFFF)
        {
	  *dst++ = 0x80 | ((cp >> 21) & 0x7F);
	  *dst++ = 0x80 | ((cp >> 14) & 0x7F);
	  *dst++ = 0x80 | ((cp >> 7) & 0x7F);
	  *dst++ = cp & 0x7F;
        }
      else
        {
	  *dst++ = 0x80 | ((cp >> 28) & 0x7F);
	  *dst++ = 0x80 | ((cp >> 21) & 0x7F);
	  *dst++ = 0x80 | ((cp >> 14) & 0x7F);
	  *dst++ = 0x80 | ((cp >> 7) & 0x7F);
	  *dst++ = cp & 0x7F;
        }
    }

  return n;
}

/* copy 'n' codepoints from stream 'src' to 'dst' */
static void
_copy_from_stream (uint32_t *dst, const uint8_t *src, size_t n)
{
  uint32_t cp = 0;

  for (; n; src++)
    {
      cp = (cp << 7) | (*src & 0x7F);
      if ((*src & 0x80) == 0)
	{
	  *dst++ = cp;
	  cp = 0;
	  n--;
	}
    }
}

static int
read_IdnaMappings (char *linep)
{
  IDNAMap *map = &idna_map[map_pos];
  char *flag, *codepoint, *mapping;
  int n;

  codepoint = _nextField (&linep);
  flag = _nextField (&linep);
  mapping = _nextField (&linep);

  if ((n = sscanf (codepoint, "%X..%X", &map->cp1, &map->cp2)) == 1)
    {
      map->cp2 = map->cp1;
    }
  else if (n != 2)
    {
      printf ("Failed to scan mapping codepoint '%s'\n", codepoint);
      return -1;
    }

  if (map->cp1 > map->cp2)
    {
      printf ("Invalid codepoint range '%s'\n", codepoint);
      return -1;
    }

  if (map_pos && map->cp1 <= idna_map[map_pos - 1].cp2)
    {
      printf ("Mapping codepoints out of order '%s'\n", codepoint);
      return -1;
    }

  if (!strcmp (flag, "valid"))
    map->valid = 1;
  else if (!strcmp (flag, "mapped"))
    map->mapped = 1;
  else if (!strcmp (flag, "disallowed"))
    map->disallowed = 1;
  else if (!strcmp (flag, "ignored"))
    map->ignored = 1;
  else if (!strcmp (flag, "deviation"))
    map->deviation = 1;
  else if (!strcmp (flag, "disallowed_STD3_mapped"))
    map->disallowed = map->disallowed_std3_mapped = 1;
  else if (!strcmp (flag, "disallowed_STD3_valid"))
    map->disallowed = map->disallowed_std3_valid = 1;
  else
    {
      printf ("Unknown flag '%s'\n", flag);
      return -1;
    }

  if (mapping && *mapping)
    {
      uint32_t cp, tmp[20], tmp2[20];
      int n, pos;
      uint8_t *stream = genmapdata + mapdata_pos;

      while ((n = sscanf (mapping, " %X%n", &cp, &pos)) == 1)
	{
	  if (mapdata_pos >= countof (genmapdata))
	    {
	      printf ("genmapdata too small - increase and retry\n");
	      break;
	    }

	  if (map->nmappings == 0)
	    {
	      map->offset = mapdata_pos;
	      if (map->offset != mapdata_pos)
		printf("offset overflow (%zu)\n", mapdata_pos);
	    }

	  tmp[map->nmappings] = cp;
	  mapdata_pos += _u32_to_stream(genmapdata + mapdata_pos, 5, &cp, 1);
//	  genmapdata[mapdata_pos++] = cp;
	  map->nmappings++;
	  mapping += pos;
	}

      /* selftest */
      _copy_from_stream(tmp2, genmapdata + map->offset, map->nmappings);
      for (int it = 0; it < map->nmappings; it++)
	if (tmp[it] != tmp2[it])
	  abort();
    }
  else if (map->mapped || map->disallowed_std3_mapped || map->deviation)
    {
      if (map->cp1 != 0x200C && map->cp1 != 0x200D)	// ZWNJ and ZWJ
	printf ("Missing mapping for '%s'\n", codepoint);
    }

  if (map_pos && map->nmappings == 0)
    {
      /* merge with previous if possible */
      IDNAMap *prev = &idna_map[map_pos - 1];
      if (prev->cp2 + 1 == map->cp1
	  && prev->nmappings == 0
	  && prev->valid == map->valid
	  && prev->mapped == map->mapped
	  && prev->ignored == map->ignored
	  && prev->deviation == map->deviation
	  && prev->disallowed == map->disallowed
	  && prev->disallowed_std3_mapped == map->disallowed_std3_mapped
	  && prev->disallowed_std3_valid == map->disallowed_std3_valid
	  && prev->transitional == map->transitional
	  && prev->nontransitional == map->nontransitional)
	{
	  prev->cp2 = map->cp2;
	  memset (map, 0, sizeof (*map));	/* clean up */
	  return 0;
	}
    }

  if (++map_pos >= countof (idna_map))
    {
      printf ("Internal map size too small\n");
      return -1;
    }

  return 0;
}

static int
_compare_map (IDNAMap * m1, IDNAMap * m2)
{
  if (m1->cp1 < m2->cp1)
    return -1;
  if (m1->cp1 > m2->cp2)
    return 1;
  return 0;
}

static int
read_NFCQC (char *linep)
{
  NFCQCMap *map = &nfcqc_map[nfcqc_pos];
  char *codepoint, *type, *check;
  int n;

  codepoint = _nextField (&linep);
  type = _nextField (&linep);
  check = _nextField (&linep);

  if (!type || strcmp (type, "NFC_QC"))
    return 0;

  if ((n = sscanf (codepoint, "%X..%X", &map->cp1, &map->cp2)) == 1)
    {
      map->cp2 = map->cp1;
    }
  else if (n != 2)
    {
      printf ("Failed to scan mapping codepoint '%s'\n", codepoint);
      return -1;
    }

  if (map->cp1 > map->cp2)
    {
      printf ("Invalid codepoint range '%s'\n", codepoint);
      return -1;
    }

  if (*check == 'N')
    map->check = 0;
  else if (*check == 'M')
    map->check = 1;
  else
    {
      printf ("NFQQC: Unknown value '%s'\n", check);
      return -1;
    }

  if (++nfcqc_pos >= countof (nfcqc_map))
    {
      printf ("Internal NFCQC map size too small\n");
      return -1;
    }

  return 0;
}

static int
_compare_map_by_maplen (IDNAMap * m1, IDNAMap * m2)
{
  if (m1->nmappings != m2->nmappings)
    return m2->nmappings - m1->nmappings;
  if (m1->cp1 < m2->cp1)
    return -1;
  if (m1->cp1 > m2->cp2)
    return 1;
  return 0;
}

static uint32_t *
_u32_memmem(uint32_t *haystack, size_t hlen, uint32_t *needle, size_t nlen)
{
  uint32_t *p;

  if (nlen == 0)
    return haystack;

  for (p = haystack; hlen >= nlen; p++, hlen--)
    {
      if (*p == *needle && (nlen == 1 || u32_cmp(p, needle, nlen) == 0))
	return p;
    }

  return NULL;
}

static uint8_t *
_u8_memmem(uint8_t *haystack, size_t hlen, uint8_t *needle, size_t nlen)
{
  uint8_t *p;

  if (nlen == 0)
    return haystack;

  for (p = haystack; hlen >= nlen; p++, hlen--)
    {
      if (*p == *needle && (nlen == 1 || memcmp(p, needle, nlen) == 0))
	return p;
    }

  return NULL;
}

static size_t
_u32_cp_stream_len(const uint8_t *stream, size_t ncp)
{
  const uint8_t *end;

  for (end = stream; ncp; end++)
    {
      if ((*end & 0x80) == 0)
	ncp--;
    }

  return end - stream;
}

/* Remove doubled mappings. With Unicode 6.3.0 the mapping data shrinks
 *  from 7272 to 4322 entries of uint32_t (29088 to 17288 bytes).
 * Converting those 4322 uin32_t values to a uint8_t stream, we decrease mapping
 *  table size from 17288 to 9153 bytes.
 */
void
_compact_idna_map(void)
{
  int it;

  /* sort into 'longest mappings first' */
  qsort (idna_map, map_pos, sizeof (IDNAMap),
	 (int (*)(const void *, const void *)) _compare_map_by_maplen);

  uint8_t *data = calloc(sizeof(uint8_t), mapdata_pos), *p;
  size_t ndata = 0, slen;

  for (it = 0; it < map_pos; it++)
    {
      IDNAMap *map = idna_map + it;

      if (!map->nmappings)
	continue;

      slen = _u32_cp_stream_len(genmapdata + map->offset, map->nmappings);

      if ((p = _u8_memmem(data, ndata, genmapdata + map->offset, slen)))
	{
	  map->offset = p - data;
	  continue;
	}

      memcpy(data + ndata, genmapdata + map->offset, slen);
      map->offset = ndata;
      ndata += slen;
    }

  memcpy(genmapdata, data, ndata);
  mapdata_pos = ndata;
  free(data);

  /* sort into 'lowest codepoint first' */
  qsort (idna_map, map_pos, sizeof (IDNAMap),
	 (int (*)(const void *, const void *)) _compare_map);
}

int
main (void)
{
  int it;

  // read IDNA mappings
  if (_scan_file (SRCDIR"/IdnaMappingTable.txt", read_IdnaMappings))
    return 1;

  _compact_idna_map();

  // read NFC QuickCheck table
  if (_scan_file (SRCDIR"/DerivedNormalizationProps.txt", read_NFCQC))
    return 1;

  qsort (nfcqc_map, nfcqc_pos, sizeof (NFCQCMap),
	 (int (*)(const void *, const void *)) _compare_map);

  printf ("/* This file is automatically generated.  DO NOT EDIT! */\n\n");
  printf ("#include <sys/types.h>\n");
  printf ("#include <stdlib.h>\n");
  printf ("#include \"tr46map.h\"\n\n");
  printf ("IDNAMap idna_map[%zu] = {\n", map_pos);
  for (it = 0; it < map_pos; it++)
    {
      IDNAMap *map = idna_map + it;
      printf ("{0x%X,0x%X,%d,%d,", map->cp1, map->cp2, map->nmappings,
	      map->offset);
      printf ("%d,", map->valid);
      printf ("%d,", map->mapped);
      printf ("%d,", map->ignored);
      printf ("%d,", map->deviation);
      printf ("%d,", map->disallowed);
      printf ("%d,", map->disallowed_std3_mapped);
      printf ("%d,", map->disallowed_std3_valid);
      printf ("%d,", map->transitional);
      printf ("%d", map->nontransitional);
      printf ("},\n");
    }
  printf ("};\n");
  printf ("const size_t map_pos = %zu;\n\n", map_pos);

  printf ("uint8_t mapdata[%zu] = {\n", mapdata_pos);
  for (it = 0; it < mapdata_pos; it++)
    {
      printf ("0x%02X,%s", genmapdata[it], it % 16 == 15 ? "\n" : "");
    }
  printf ("};\n\n");

  printf ("NFCQCMap nfcqc_map[%zu] = {\n", nfcqc_pos);
  for (it = 0; it < nfcqc_pos; it++)
    {
      NFCQCMap *map = nfcqc_map + it;
      printf ("{0x%X,0x%X,%d},\n", map->cp1, map->cp2, map->check);
    }
  printf ("};\n");
  printf ("const size_t nfcqc_pos = %zu;\n\n", nfcqc_pos);

  printf ("/* arg1 works with 'uint32_t *' and with 'IDNAMap *' */\n"
	  "int _compare_map(uint32_t *c, IDNAMap *m2)\n"
	  "{\n"
	  "	if (*c < m2->cp1)\n"
	  "		return -1;\n"
	  "	if (*c > m2->cp2)\n"
	  "		return 1;\n"
	  "	return 0;\n"
	  "}\n\n"
	  "IDNAMap *_get_map(uint32_t c)\n"
	  "{\n"
	  "	return bsearch(&c, idna_map, map_pos, sizeof(IDNAMap), (int(*)(const void *, const void *))_compare_map);\n"
	  "}\n\n"
	  "NFCQCMap *_get_nfcqc_map(uint32_t c)\n"
	  "{\n"
	  "	return bsearch(&c, nfcqc_map, nfcqc_pos, sizeof(NFCQCMap), (int(*)(const void *, const void *))_compare_map);\n"
	  "}\n");

  return 0;
}
