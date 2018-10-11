
/**
 * \file flush.c
 *
 * \brief Segment flush routines.
 *
 * This program is free software under the GNU General Public License
 * (>=v2). Read the file COPYING that comes with GRASS for details.
 *
 * \author GRASS GIS Development Team
 *
 * \date 2005-2009
 */

#include "local_proto.h"


/**
 * \fn int Segment_flush (SEGMENT *SEG)
 *
 * \brief Flush pending updates to disk.
 *
 * Forces all pending updates generated by <i>Segment_put()</i> to be 
 * written to the segment file <b>seg</b>. Must be called after the 
 * final <i>Segment_put()</i> to force all pending updates to disk. Must 
 * also be called before the first call to <i>Segment_get_row</i>.
 *
 * \param[in] SEG segment
 * \return always returns 0
 */

int Segment_flush(SEGMENT * SEG)
{
    int i;

    if (SEG->scb) {
	for (i = 0; i < SEG->nseg; i++)
	    if (SEG->scb[i].n >= 0 && SEG->scb[i].dirty)
		seg_pageout(SEG, i);
    }

    return 0;
}
