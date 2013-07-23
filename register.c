/* register.c - implementation of IDNA2008 register functions
   Copyright (C) 2011-2013 Simon Josefsson

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

#include <errno.h>		/* errno */
#include <stdlib.h>		/* free */

#include "punycode.h"

#include "uniconv.h"		/* u8_strconv_from_locale */
#include "unistr.h"		/* u32_to_u8 */

#include "idna.h"		/* _idn2_label_test */

/**
 * idn2_register_u8:
 * @ulabel: input zero-terminated UTF-8 and Unicode NFC string, or NULL.
 * @alabel: input zero-terminated ACE encoded string (xn--), or NULL.
 * @insertname: newly allocated output variable with name to register in DNS.
 * @flags: optional #idn2_flags to modify behaviour.
 *
 * Perform IDNA2008 register string conversion on domain label @ulabel
 * and @alabel, as described in section 4 of RFC 5891.  Note that the
 * input @ulabel must be encoded in UTF-8 and be in Unicode NFC form.
 *
 * Pass %IDN2_NFC_INPUT in @flags to convert input @ulabel to NFC form
 * before further processing.
 *
 * It is recommended to supply both @ulabel and @alabel for better
 * error checking, but supplying just one of them will work.  Passing
 * in only @alabel is better than only @ulabel.  See RFC 5891 section
 * 4 for more information.
 *
 * Returns: On successful conversion %IDN2_OK is returned, when the
 *   given @ulabel and @alabel does not match each other
 *   %IDN2_UALABEL_MISMATCH is returned, when either of the input
 *   labels are too long %IDN2_TOO_BIG_LABEL is returned, when @alabel
 *   does does not appear to be a proper A-label %IDN2_INVALID_ALABEL
 *   is returned, or another error code is returned.
 **/
int
idn2_register_u8 (const uint8_t * ulabel, const uint8_t * alabel,
		  uint8_t ** insertname, int flags)
{
  int rc;

  if (ulabel == NULL && alabel == NULL)
    {
      *insertname = NULL;
      return IDN2_OK;
    }

  if (ulabel && strlen (ulabel) >= IDN2_LABEL_MAX_LENGTH)
    return IDN2_TOO_BIG_LABEL;
  if (alabel && strlen (alabel) >= IDN2_LABEL_MAX_LENGTH)
    return IDN2_TOO_BIG_LABEL;

  if (alabel && !_idn2_ascii_p (alabel, strlen (alabel)))
    return IDN2_INVALID_ALABEL;

  if (alabel)
    {
      size_t alabellen = strlen (alabel), u32len = IDN2_LABEL_MAX_LENGTH * 4;
      uint32_t u32[IDN2_DOMAIN_MAX_LENGTH * 4];
      uint8_t *tmp;
      uint8_t u8[IDN2_DOMAIN_MAX_LENGTH + 1];
      size_t u8len;

      if (alabellen <= 4)
	return IDN2_INVALID_ALABEL;
      if (alabel[0] != 'x'
	  || alabel[1] != 'n' || alabel[2] != '-' || alabel[3] != '-')
	return IDN2_INVALID_ALABEL;

      rc = _idn2_punycode_decode (alabellen - 4, alabel + 4,
				  &u32len, u32, NULL);
      if (rc != IDN2_OK)
	return rc;

      u8len = sizeof (u8);
      if (u32_to_u8 (u32, u32len, u8, &u8len) == NULL)
	return IDN2_ENCODING_ERROR;
      u8[u8len] = '\0';

      if (ulabel)
	{
	  if (strcmp (ulabel, u8) != 0)
	    return IDN2_UALABEL_MISMATCH;
	}

      rc = idn2_register_u8 (u8, NULL, &tmp, 0);
      if (rc != IDN2_OK)
	return rc;

      rc = strcmp (alabel, tmp);
      free (tmp);
      if (rc != 0)
	return IDN2_UALABEL_MISMATCH;

      *insertname = strdup (alabel);
    }
  else				/* ulabel only */
    {
      uint32_t *u32;
      size_t u32len;
      size_t tmpl;

      *insertname = malloc (IDN2_LABEL_MAX_LENGTH + 1);
      if (*insertname == NULL)
	return IDN2_MALLOC;

      if (_idn2_ascii_p (ulabel, strlen (ulabel)))
	{
	  strcpy (*insertname, ulabel);
	  return IDN2_OK;
	}

      rc = _idn2_u8_to_u32_nfc (ulabel, strlen (ulabel), &u32, &u32len,
				flags & IDN2_NFC_INPUT);
      if (rc != IDN2_OK)
	{
	  free (*insertname);
	  return rc;
	}

      rc = _idn2_label_test (TEST_NFC
			     | TEST_DISALLOWED
			     | TEST_UNASSIGNED
			     | TEST_2HYPHEN
			     | TEST_HYPHEN_STARTEND
			     | TEST_LEADING_COMBINING
			     | TEST_CONTEXTJ_RULE
			     | TEST_CONTEXTO_RULE | TEST_BIDI, u32, u32len);
      if (rc != IDN2_OK)
	{
	  free (*insertname);
	  free (u32);
	  return rc;
	}

      (*insertname)[0] = 'x';
      (*insertname)[1] = 'n';
      (*insertname)[2] = '-';
      (*insertname)[3] = '-';

      tmpl = IDN2_LABEL_MAX_LENGTH - 4;
      rc = _idn2_punycode_encode (u32len, u32, NULL, &tmpl, *insertname + 4);
      free (u32);
      if (rc != IDN2_OK)
	{
	  free (*insertname);
	  return rc;
	}

      (*insertname)[4 + tmpl] = '\0';
    }

  return IDN2_OK;
}

/**
 * idn2_register_ul:
 * @ulabel: input zero-terminated locale encoded string, or NULL.
 * @alabel: input zero-terminated ACE encoded string (xn--), or NULL.
 * @insertname: newly allocated output variable with name to register in DNS.
 * @flags: optional #idn2_flags to modify behaviour.
 *
 * Perform IDNA2008 register string conversion on domain label @ulabel
 * and @alabel, as described in section 4 of RFC 5891.  Note that the
 * input @ulabel is assumed to be encoded in the locale's default
 * coding system, and will be transcoded to UTF-8 and NFC normalized
 * by this function.
 *
 * It is recommended to supply both @ulabel and @alabel for better
 * error checking, but supplying just one of them will work.  Passing
 * in only @alabel is better than only @ulabel.  See RFC 5891 section
 * 4 for more information.
 *
 * Returns: On successful conversion %IDN2_OK is returned, when the
 *   given @ulabel and @alabel does not match each other
 *   %IDN2_UALABEL_MISMATCH is returned, when either of the input
 *   labels are too long %IDN2_TOO_BIG_LABEL is returned, when @alabel
 *   does does not appear to be a proper A-label %IDN2_INVALID_ALABEL
 *   is returned, or another error code is returned.
 **/
int
idn2_register_ul (const char *ulabel, const char *alabel,
		  char **insertname, int flags)
{
  uint8_t *utf8ulabel = u8_strconv_from_locale (ulabel);
  int rc;

  if (utf8ulabel == NULL)
    {
      if (errno == ENOMEM)
	return IDN2_MALLOC;
      return IDN2_ICONV_FAIL;
    }

  rc = idn2_register_u8 (utf8ulabel, (const uint8_t *) alabel,
			 (uint8_t **) insertname, flags | IDN2_NFC_INPUT);

  free (utf8ulabel);

  return rc;
}
