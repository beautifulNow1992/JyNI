/*
 * Copyright of JyNI:
 * Copyright (c) 2013, 2014, 2015 Stefan Richthofer.  All rights reserved.
 *
 *
 * Copyright of Python and Jython:
 * Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
 * 2011, 2012, 2013, 2014, 2015 Python Software Foundation.  All rights reserved.
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


package JyNI.gc;

public interface TraversableGCHead extends JyGCHead {
	/**
	 * Do not call this method. It is for internal use and only
	 * public because interfaces require this.
	 * 
	 * Note:
	 * links must be of one of these types:
	 * JyGCHead, JyGCHead[] or Iterable<JyGCHead>
	 */
	public void setLinks(Object links);

	/**
	 * Do not call this method. It is for internal use and only
	 * public because interfaces require this.
	 *
	 * In var-size case this operation is only supported if the
	 * backend is a Collection. (Since an arbitrary iterable has
	 * no add- or set-operation.)
	 *
	 * @return -2 if links == null, -1 for invalid index, 0 otherwise.
	 */
	public int setLink(int index, JyGCHead link);

	/**
	 * Do not call this method. It is for internal use and only
	 * public because interfaces require this.
	 *
	 * In var-size case this operation is only supported if the
	 * backend is a List. In other cases it is not supported at all.
	 *
	 * @return -2 if links == null, -1 for invalid index, 0 otherwise.
	 */
	public int insertLink(int index, JyGCHead link);
	
	/**
	 * Do not call this method. It is for internal use and only
	 * public because interfaces require this.
	 *
	 * Removes the indexed element from GC's responsibility.
	 * In var-size case, this actually removes the element.
	 * In fixed-size case this sets the element-position to null.
	 * In single-link case it sets the links-variable to null.
	 *
	 * @return -2 if links == null, -1 for invalid index, 0 otherwise.
	 */
	public int clearLink(int index);
	
	/**
	 * Do not call this method. It is for internal use and only
	 * public because interfaces require this.
	 *
	 * Removes the all elements with index >= startIndex from GC's responsibility.
	 * In var-size case, this actually removes the elements.
	 * In fixed-size case this sets the element-positions to null.
	 * In single-link case it sets the links-variable to null.
	 *
	 * @return -2 if links == null, -1 for invalid startIndex, number of cleared indices otherwise.
	 */
	public int clearLinksFromIndex(int startIndex);
	public int jyTraverse(JyVisitproc visit, Object arg);
}
