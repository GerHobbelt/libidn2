/* lookup.c - implementation of IDNA2008 lookup functions
   Copyright (C) 2011 Simon Josefsson

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <config.h>

#include "idn2.h"

#include <errno.h> /* errno */
#include <stdlib.h> /* free */

#include "punycode.h"

#include "uniconv.h" /* u8_strconv_from_locale */
#include "unistr.h" /* u8_to_u32 */
#include "uninorm.h" /* u32_normalize */

#include "idna.h" /* _idn2_label_test */

static int
label (const uint8_t *src, size_t srclen,
       uint8_t *dst, size_t *dstlen, /* assumed to be 63 */
       int flags)
{
  size_t plen;
  uint32_t *p;
  int rc;

  if (srclen > 63)
    return IDN2_TOO_BIG_LABEL;

  {
    size_t i;
    bool ascii = true;
    int rc;

    for (i = 0; i < srclen; i++)
      if (src[i] >= 0x80)
	ascii = false;

    if (ascii)
      {
	memcpy (dst, src, srclen);
	*dstlen = srclen;
	return IDN2_OK;
      }
  }

  if (flags & IDN2_ALABEL_ROUNDTRIP)
    /* FIXME: Conversion from the A-label and testing that the result is
       a U-label SHOULD be performed if the domain name will later be
       presented to the user in native character form */
    return -1;

  p = u8_to_u32 (src, srclen, NULL, &plen);
  if (p == NULL)
    {
      if (errno == ENOMEM)
	return IDN2_MALLOC;
      return IDN2_ENCODING_ERROR;
    }

  if (flags & IDN2_NFC_INPUT)
    {
      size_t tmplen;
      uint32_t *tmp = u32_normalize (UNINORM_NFC, p, plen, NULL, &tmplen);
      free (p);
      if (tmp == NULL)
	{
	  if (errno == ENOMEM)
	    return IDN2_MALLOC;
	  return IDN2_NFC;
	}

      p = tmp;
      plen = tmplen;
    }

  rc = _idn2_label_test (TEST_NFC |
			 TEST_2HYPHEN |
			 TEST_LEADING_COMBINING |
			 TEST_DISALLOWED |
			 TEST_CONTEXTJ_RULE |
			 TEST_CONTEXTO_WITH_RULE |
			 TEST_UNASSIGNED |
			 TEST_BIDI, p, plen);
  if (rc != IDN2_OK)
    return rc;

  char out[63];
  size_t tmpl;

  dst[0] = 'x';
  dst[1] = 'n';
  dst[2] = '-';
  dst[3] = '-';

  tmpl = *dstlen - 4;
  rc = _idn2_punycode_encode (plen, p, NULL, &tmpl, dst + 4);
  if (rc != IDN2_OK)
    return rc;

  *dstlen = 4 + tmpl;

  return IDN2_OK;
}

/**
 * idn2_lookup_u8:
 * @src: input zero-terminated UTF-8 string in Unicode NFC normalized form.
 * @lookupname: newly allocated output variable with name to lookup in DNS.
 * @flags: optional #Idn2_flags to modify behaviour.
 *
 * Perform IDNA2008 lookup string conversion on input @src, as
 * described in section 5 of RFC 5891.  Note that the input string
 * must be encoded in UTF-8 and be in Unicode NFC form.
 *
 * Pass %IDN2_NFC_INPUT in @flags to convert input to NFC form before
 * further processing.  Pass %IDN2_ALABEL_ROUNDTRIP in @flags to
 * convert any input A-labels to U-labels and perform additional
 * testing.  Multiple flags may be specified by binary or:ing them
 * together, for example %IDN2_NFC_INPUT | %IDN2_ALABEL_ROUNDTRIP.
 *
 * Returns: On successful conversion %IDN2_OK is returned, otherwise
 *   an error code is returned.
 **/
int
idn2_lookup_u8 (const uint8_t *src, uint8_t **lookupname, int flags)
{
  size_t lookupnamelen = 0;
  int rc;

  if (src == NULL)
    return IDN2_OK;

  if (strlen (src) > IDN2_DOMAIN_MAX_LENGTH)
    return IDN2_TOO_BIG_DOMAIN;

  *lookupname = malloc (IDN2_DOMAIN_MAX_LENGTH + 1);
  if (*lookupname == NULL)
    return IDN2_MALLOC;

  do
    {
      const uint8_t *end = strchrnul (src, '.');
      /* XXX Do we care about non-U+002E dots such as U+3002, U+FF0E
	 and U+FF61 here?  Perhaps when IDN2_NFC_INPUT? */
      size_t labellen = end - src;
      uint8_t tmp[IDN2_LABEL_MAX_LENGTH];
      size_t tmplen = IDN2_LABEL_MAX_LENGTH;
      int rc;

      rc = label (src, labellen, tmp, &tmplen, flags);
      if (rc != IDN2_OK)
	{
	  free (*lookupname);
	  return rc;
	}

      if (lookupnamelen + tmplen > IDN2_DOMAIN_MAX_LENGTH)
	{
	  free (*lookupname);
	  return IDN2_TOO_BIG_DOMAIN;
	}

      memcpy (*lookupname + lookupnamelen, tmp, tmplen);
      lookupnamelen += tmplen;

      if (*end == '.')
	{
	  if (lookupnamelen + 1 > IDN2_DOMAIN_MAX_LENGTH)
	    {
	      free (*lookupname);
	      return IDN2_TOO_BIG_DOMAIN;
	    }

	  (*lookupname)[lookupnamelen] = '.';
	  lookupnamelen++;
	}
      (*lookupname)[lookupnamelen] = '\0';

      src = end;
    }
  while (*src++);

  return IDN2_OK;
}

/**
 * idn2_lookup_ul:
 * @src: input zero-terminated locale encoded string.
 * @lookupname: newly allocated output variable with name to lookup in DNS.
 * @flags: optional #Idn2_flags to modify behaviour.
 *
 * Perform IDNA2008 lookup string conversion on input @src, as
 * described in section 5 of RFC 5891.  Note that the input is assumed
 * to be encoded in the locale's default coding system, and will be
 * transcoded to UTF-8 and NFC normalized by this function.
 *
 * Pass %IDN2_ALABEL_ROUNDTRIP in @flags to convert any input A-labels
 * to U-labels and perform additional testing.
 *
 * Returns: On successful conversion %IDN2_OK is returned, otherwise
 *   an error code is returned.
 **/
int
idn2_lookup_ul (const char *src, char **lookupname, int flags)
{
  uint8_t *utf8src = u8_strconv_from_locale (src);
  if (utf8src == NULL)
    {
      if (errno == ENOMEM)
	return IDN2_MALLOC;
      return IDN2_ICONV_FAIL;
    }

  int rc = idn2_lookup_u8 (utf8src, (uint8_t **) lookupname,
			   flags | IDN2_NFC_INPUT);

  free (utf8src);

  return rc;
}
