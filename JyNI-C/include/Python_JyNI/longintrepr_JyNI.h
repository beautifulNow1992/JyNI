/* This File is based on longintrepr.h from CPython 2.7.3 release.
 * It has been modified to suite JyNI needs.
 *
 * Copyright of the original file:
 * Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
 * 2011, 2012, 2013 Python Software Foundation.  All rights reserved.
 *
 * Copyright of JyNI:
 * Copyright (c) 2013 Stefan Richthofer.  All rights reserved.
 *
 *
 * This file is part of JyNI.
 *
 * JyNI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JyNI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with JyNI.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Linking this library statically or dynamically with other modules is
 * making a combined work based on this library.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this library with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this library, you may extend
 * this exception to your version of the library, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 */


#ifndef Py_LONGINTREPR_H
#define Py_LONGINTREPR_H
#ifdef __cplusplus
extern "C" {
#endif


/* This is published for the benefit of "friend" marshal.c only. */

/* Parameters of the long integer representation.  There are two different
   sets of parameters: one set for 30-bit digits, stored in an unsigned 32-bit
   integer type, and one set for 15-bit digits with each digit stored in an
   unsigned short.  The value of PYLONG_BITS_IN_DIGIT, defined either at
   configure time or in pyport.h, is used to decide which digit size to use.

   Type 'digit' should be able to hold 2*PyLong_BASE-1, and type 'twodigits'
   should be an unsigned integer type able to hold all integers up to
   PyLong_BASE*PyLong_BASE-1.  x_sub assumes that 'digit' is an unsigned type,
   and that overflow is handled by taking the result modulo 2**N for some N >
   PyLong_SHIFT.  The majority of the code doesn't care about the precise
   value of PyLong_SHIFT, but there are some notable exceptions:

   - long_pow() requires that PyLong_SHIFT be divisible by 5

   - PyLong_{As,From}ByteArray require that PyLong_SHIFT be at least 8

   - long_hash() requires that PyLong_SHIFT is *strictly* less than the number
     of bits in an unsigned long, as do the PyLong <-> long (or unsigned long)
     conversion functions

   - the long <-> size_t/Py_ssize_t conversion functions expect that
     PyLong_SHIFT is strictly less than the number of bits in a size_t

   - the marshal code currently expects that PyLong_SHIFT is a multiple of 15

  The values 15 and 30 should fit all of the above requirements, on any
  platform.
*/

#if PYLONG_BITS_IN_DIGIT == 30
#if !(defined HAVE_UINT64_T && defined HAVE_UINT32_T &&          \
      defined HAVE_INT64_T && defined HAVE_INT32_T)
#error "30-bit long digits requested, but the necessary types are not available on this platform"
#endif
typedef PY_UINT32_T digit;
typedef PY_INT32_T sdigit; /* signed variant of digit */
typedef PY_UINT64_T twodigits;
typedef PY_INT64_T stwodigits; /* signed variant of twodigits */
#define PyLong_SHIFT	30
#define _PyLong_DECIMAL_SHIFT	9 /* max(e such that 10**e fits in a digit) */
#define _PyLong_DECIMAL_BASE	((digit)1000000000) /* 10 ** DECIMAL_SHIFT */
#elif PYLONG_BITS_IN_DIGIT == 15
typedef unsigned short digit;
typedef short sdigit; /* signed variant of digit */
typedef unsigned long twodigits;
typedef long stwodigits; /* signed variant of twodigits */
#define PyLong_SHIFT	15
#define _PyLong_DECIMAL_SHIFT	4 /* max(e such that 10**e fits in a digit) */
#define _PyLong_DECIMAL_BASE	((digit)10000) /* 10 ** DECIMAL_SHIFT */
#else
#error "PYLONG_BITS_IN_DIGIT should be 15 or 30"
#endif
#define PyLong_BASE	((digit)1 << PyLong_SHIFT)
#define PyLong_MASK	((digit)(PyLong_BASE - 1))

/* b/w compatibility with Python 2.5 */
#define SHIFT	PyLong_SHIFT
#define BASE	PyLong_BASE
#define MASK	PyLong_MASK

#if PyLong_SHIFT % 5 != 0
#error "longobject.c requires that PyLong_SHIFT be divisible by 5"
#endif

/* Long integer representation.
   The absolute value of a number is equal to
   	SUM(for i=0 through abs(ob_size)-1) ob_digit[i] * 2**(SHIFT*i)
   Negative numbers are represented with ob_size < 0;
   zero is represented by ob_size == 0.
   In a normalized number, ob_digit[abs(ob_size)-1] (the most significant
   digit) is never zero.  Also, in all cases, for all valid i,
   	0 <= ob_digit[i] <= MASK.
   The allocation function takes care of allocating extra memory
   so that ob_digit[0] ... ob_digit[abs(ob_size)-1] are actually available.

   CAUTION:  Generic code manipulating subtypes of PyVarObject has to
   aware that longs abuse  ob_size's sign bit.
*/

struct _longobject {
	PyObject_VAR_HEAD
	digit ob_digit[1];
};

PyAPI_FUNC(PyLongObject *) _PyLong_New(Py_ssize_t);

/* Return a copy of src. */
PyAPI_FUNC(PyObject *) _PyLong_Copy(PyLongObject *src);

#ifdef __cplusplus
}
#endif
#endif /* !Py_LONGINTREPR_H */