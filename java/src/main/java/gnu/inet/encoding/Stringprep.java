/* Copyright (C) 2004-2012 Free Software Foundation, Inc.
   Author: Oliver Hitz

   This file is part of GNU Libidn.

   GNU Libidn is free software: you can redistribute it and/or
   modify it under the terms of either:

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at
       your option) any later version.

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at
       your option) any later version.

   or both in parallel, as here.

   GNU Libidn is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see <http://www.gnu.org/licenses/>. */

package gnu.inet.encoding;

import java.util.Arrays;

/**
 * This class offers static methods for preparing internationalized
 * strings. It supports the following stringprep profiles:
 * <ul>
 * <li>RFC3491 nameprep
 * <li>RFC3920 XMPP nodeprep and resourceprep
 * </ul>
 * Note that this implementation only supports 16-bit Unicode code
 * points.
 */
public class Stringprep
{
  /**
   * Preps a name according to the Stringprep profile defined in
   * RFC3491. Unassigned code points are not allowed.
   *
   * @param input the name to prep.
   * @return the prepped name.
   * @throws StringprepException If the name cannot be prepped with
   * this profile.
   * @throws NullPointerException If the name is null.
   */
  public static String nameprep(String input)
    throws StringprepException,
	   NullPointerException
  {
    return nameprep(input, false);
  }

  /**
   * Preps a name according to the Stringprep profile defined in
   * RFC3491.
   *
   * @param input the name to prep.
   * @param allowUnassigned true if the name may contain unassigned
   * code points.
   * @return the prepped name.
   * @throws StringprepException If the name cannot be prepped with
   * this profile.
   * @throws NullPointerException If the name is null.
   */
  public static String nameprep(String input, boolean allowUnassigned)
    throws StringprepException,
	   NullPointerException
  {
    if (input == null) {
      throw new NullPointerException();
    }

    if (!allowUnassigned && RANGE_A1.containsAnyCodePoint(input)) {
      throw new StringprepException(StringprepException.CONTAINS_UNASSIGNED);
    }

    StringBuilder s = new StringBuilder(input);

    filter(s, RFC3454.B1);
    map(s, RFC3454.B2search, RFC3454.B2replace);

    s = new StringBuilder(NFKC.normalizeNFKC(s.toString()));
    // B.3 is only needed if NFKC is not used, right?
    // map(s, RFC3454.B3search, RFC3454.B3replace);
    if (RANGE_C3_to_C8_C12_C22.containsAnyCodePoint(s)) {
      // Table C.9 only contains code points > 0xFFFF which Java
      // doesn't handle
      throw new StringprepException(StringprepException.CONTAINS_PROHIBITED);
    }

    // Bidi handling
    boolean r = RANGE_D1.containsAnyCodePoint(s);
    boolean l = RANGE_D2.containsAnyCodePoint(s);

    // RFC 3454, section 6, requirement 1: already handled above (table C.8)

    // RFC 3454, section 6, requirement 2
    if (r && l) {
      throw new StringprepException(StringprepException.BIDI_BOTHRAL);
    }

    // RFC 3454, section 6, requirement 3
    if (r) {
      if (!RANGE_D1.containsAnyCodePoint(Character.toString(s.charAt(0))) ||
	  !RANGE_D1.containsAnyCodePoint(Character.toString(s.charAt(s.length()-1)))) {
	throw new StringprepException(StringprepException.BIDI_LTRAL);
      }
    }

    return s.toString();
  }

  private static final RangeSet RANGE_A1 =
	  RangeSet.builder().addRanges(RFC3454.A1)
	  		    .build();

  private static final RangeSet RANGE_D1 =
	  RangeSet.builder().addRanges(RFC3454.D1)
		  .build();

  private static final RangeSet RANGE_D2 =
	  RangeSet.builder().addRanges(RFC3454.D2)
		  .build();


  private static final RangeSet RANGE_C3_to_C8_C12_C22 =
	  RangeSet.builder().addRanges(RFC3454.C12)
			    .addRanges(RFC3454.C22)
			    .addRanges(RFC3454.C3)
			    .addRanges(RFC3454.C4)
			    .addRanges(RFC3454.C5)
			    .addRanges(RFC3454.C6)
			    .addRanges(RFC3454.C7)
			    .addRanges(RFC3454.C8)
			    // TODO Add C9 table now, proper unicode support now
			    // Temporary rejection of all "unsupported" in java 1.4
		  	    .addRange(new RangeSet.Range(0xffff, 0x10ffff))
	  	            .build();

  /**
   * Characters prohibited by RFC3920 nodeprep that aren't defined as
   * part of the RFC3454 tables.
   */
  private static final char [] RFC3920_NODEPREP_PROHIBIT = new char [] {
    '\u0022', '\u0026', '\'',     '\u002F',
    '\u003A', '\u003C', '\u003E', '\u0040'
  };

  /**
   * Preps a node name according to the Stringprep profile defined in
   * RFC3920. Unassigned code points are not allowed.
   *
   * @param input the node name to prep.
   * @return the prepped node name.
   * @throws StringprepException If the node name cannot be prepped
   * with this profile.
   * @throws NullPointerException If the node name is null.
   */
  public static String nodeprep(String input)
    throws StringprepException,
	   NullPointerException
  {
    return nodeprep(input, false);
  }

  /**
   * Preps a node name according to the Stringprep profile defined in
   * RFC3920.
   *
   * @param input the node name to prep.
   * @param allowUnassigned true if the node name may contain
   * unassigned code points.
   * @return the prepped node name.
   * @throws StringprepException If the node name cannot be prepped
   * with this profile.
   * @throws NullPointerException If the node name is null.
   */
  public static String nodeprep(String input, boolean allowUnassigned)
    throws StringprepException,
	   NullPointerException
  {
    if (input == null) {
      throw new NullPointerException();
    }

    if (!allowUnassigned && RANGE_A1.containsAnyCodePoint(input)) {
      throw new StringprepException(StringprepException.CONTAINS_UNASSIGNED);
    }

    StringBuilder s = new StringBuilder(input);

    filter(s, RFC3454.B1);
    map(s, RFC3454.B2search, RFC3454.B2replace);

    s = new StringBuilder(NFKC.normalizeNFKC(s.toString()));
    if (RANGE_C3_TO_C8_C11_12_21_22_NP_PROHIB.containsAnyCodePoint(s))
    {
      throw new StringprepException(StringprepException.CONTAINS_PROHIBITED);
    }

    // Bidi handling
    boolean r = contains(s, RFC3454.D1);
    boolean l = contains(s, RFC3454.D2);

    // RFC 3454, section 6, requirement 1: already handled above (table C.8)

    // RFC 3454, section 6, requirement 2
    if (r && l) {
      throw new	StringprepException(StringprepException.BIDI_BOTHRAL);
    }
    
    // RFC 3454, section 6, requirement 3
    if (r) {
      if (!contains(s.charAt(0), RFC3454.D1) ||
	  !contains(s.charAt(s.length() - 1), RFC3454.D1)) {
	throw new StringprepException(StringprepException.BIDI_LTRAL);
      }
    }
    
    return s.toString();
  }

  private static final RangeSet RANGE_C3_TO_C8_C11_12_21_22_NP_PROHIB =
	  RangeSet.builder().addRanges(RFC3454.C3)
			    .addRanges(RFC3454.C4)
			    .addRanges(RFC3454.C5)
			    .addRanges(RFC3454.C6)
			    .addRanges(RFC3454.C7)
			    .addRanges(RFC3454.C8)
			    .addRanges(RFC3454.C11)
			    .addRanges(RFC3454.C12)
			    .addRanges(RFC3454.C21)
			    .addRanges(RFC3454.C22)
		    	    .addRanges(RFC3920_NODEPREP_PROHIBIT)
			    // TODO Add C9 table now, proper unicode support now
			    // Temporary rejection of all "unsupported" in java 1.4
	  		    .addRange(new RangeSet.Range(0xffff, 0x10ffff))
		  	    .build();


  /**
   * Preps a resource name according to the Stringprep profile defined
   * in RFC3920. Unassigned code points are not allowed.
   *
   * @param input the resource name to prep.
   * @return the prepped node name.
   * @throws StringprepException If the resource name cannot be prepped
   * with this profile.
   * @throws NullPointerException If the resource name is null.
   */
  public static String resourceprep(String input)
    throws StringprepException,
	   NullPointerException
  {
    return resourceprep(input, false);
  }

  /**
   * Preps a resource name according to the Stringprep profile defined
   * in RFC3920.
   *
   * @param input the resource name to prep.
   * @param allowUnassigned true if the resource name may contain
   * unassigned code points.
   * @return the prepped node name.
   * @throws StringprepException If the resource name cannot be prepped
   * with this profile.
   * @throws NullPointerException If the resource name is null.
   */
  public static String resourceprep(String input, boolean allowUnassigned)
    throws StringprepException,
	   NullPointerException
  {
    if (input == null) {
      throw new NullPointerException();
    }

    StringBuilder s = new StringBuilder(input);
    
    if (!allowUnassigned && contains(s, RFC3454.A1)) {
      throw new StringprepException(StringprepException.CONTAINS_UNASSIGNED);
    }

    filter(s, RFC3454.B1);
    
    s = new StringBuilder(NFKC.normalizeNFKC(s.toString()));

    if (RANGE_C3_to_C8_C12_C21_C22.containsAnyCodePoint(s)) {
      // Table C.9 only contains code points > 0xFFFF which Java
      // doesn't handle

      throw new StringprepException(StringprepException.CONTAINS_PROHIBITED);
    }
    
    // Bidi handling
    boolean r = contains(s, RFC3454.D1);
    boolean l = contains(s, RFC3454.D2);
    
    // RFC 3454, section 6, requirement 1: already handled above (table C.8)
    
    // RFC 3454, section 6, requirement 2
    if (r && l) {
      throw new	StringprepException(StringprepException.BIDI_BOTHRAL);
    }

    // RFC 3454, section 6, requirement 3
    if (r) {
      if (!contains(s.charAt(0), RFC3454.D1) ||
	  !contains(s.charAt(s.length() - 1), RFC3454.D1)) {
	throw new StringprepException(StringprepException.BIDI_LTRAL);
      }
    }
    
    return s.toString();
  }

  private static final RangeSet RANGE_C3_to_C8_C12_C21_C22 =
	  RangeSet.builder().addRanges(RFC3454.C12)
		  .addRanges(RFC3454.C21)
		  .addRanges(RFC3454.C22)
		  .addRanges(RFC3454.C3)
		  .addRanges(RFC3454.C4)
		  .addRanges(RFC3454.C5)
		  .addRanges(RFC3454.C6)
		  .addRanges(RFC3454.C7)
		  .addRanges(RFC3454.C8)
		  // TODO Add C9 table now, proper unicode support now
		  // Temporary rejection of all "unsupported" in java 1.4
		  .addRange(new RangeSet.Range(0xffff, 0x10ffff))
		  .build();


  static boolean contains(StringBuilder s, char[] p)
  {
    for (int i = 0; i < p.length; i++) {
      char c = p[i];
      for (int j = 0; j < s.length(); j++) {
	if (c == s.charAt(j)) {
	  return true;
	}
      }
    }
    return false;
  }

  // TODO Reduce StringBuilder.charAt(): 60% and length(): 13%
  // TODO By pre-sorting chars of s, p and s can be iterated through in lockstep
  // TODO Implement+use RangeSet.containsAnyCodePoint(CharSequence/String)
  static boolean contains(StringBuilder s, char[][] p)
  {
    final int sLength = s.length();
    for (int i = 0; i < p.length; i++) {
      char[] r = p[i];
      if (1 == r.length) {
	char c = r[0];
	boolean sAfterC = false;
	for (int j = 0; j < sLength; j++) {
	  char charAtJ = s.charAt(j);
	  if (charAtJ == c) {
	    return true;
	  }
	  if (charAtJ > c) {
	    sAfterC = true;
	  }
	}
	if (!sAfterC) {
	  return false;
	}
      } else if (2 == r.length) {
	char f = r[0];
	char t = r[1];
	for (int j = 0; j < sLength; j++) {
	  final char charAtJ = s.charAt(j);
	  if (f <= charAtJ && t >= charAtJ) {
	    return true;
	  }
	}
      }
    }
    return false;
  }

  static boolean contains(char c, char[][] p)
  {
    for (int i = 0; i < p.length; i++) {
      char[] r = p[i];
      if (1 == r.length) {
	if (c == r[0]) {
	  return true;
	}
      } else if (2 == r.length) {
	char f = r[0];
	char t = r[1];
	if (f <= c && t >= c) {
	  return true;
	}
      }
    }
    return false;
  }

  static void filter(StringBuilder s, char[] f)
  {
    for (int i = 0; i < f.length; i++) {
      char c = f[i];

      int j = 0;
      while (j < s.length()) {
	if (c == s.charAt(j)) {
	  s.deleteCharAt(j);
	} else {
	  j++;
	}
      }
    }
  }

  static void filter(StringBuilder s, char[][] f)
  {
    for (int i = 0; i < f.length; i++) {
      char[] r = f[i];

      if (1 == r.length) {
	char c = r[0];

	int j = 0;
	while (j < s.length()) {
	  if (c == s.charAt(j)) {
	    s.deleteCharAt(j);
	  } else {
	    j++;
	  }
	}
      } else if (2 == r.length) {
	char from = r[0];
	char to = r[1];

	int j = 0;
	while (j < s.length()) {
	  if (from <= s.charAt(j) && to >= s.charAt(j)) {
	    s.deleteCharAt(j);
	  } else {
	    j++;
	  }
	}
      }
    }
  }

  static void map(StringBuilder s, char[] search, String[] replace)
  {
    for (int i = 0; i < s.length(); i++) {
      char c = s.charAt(i);
      int mapIndex = Arrays.binarySearch(search, c);
      if (mapIndex >= 0) {
	String replacement = replace[mapIndex];
	s.replace(i, i + 1, replacement);
	i += replacement.length() - 1;
      }
    }
  }
}