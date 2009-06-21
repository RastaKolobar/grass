
/****************************************************************************
 *
 * MODULE:       r.cats
 *
 * AUTHOR(S):    Michael Shapiro - CERL
 *
 * PURPOSE:      Prints category values and labels associated with
 *               user-specified raster map layers.
 *
 * COPYRIGHT:    (C) 2006 by the GRASS Development Team
 *
 *               This program is free software under the GNU General Public
 *               License (>=v2). Read the file COPYING that comes with GRASS
 *               for details.
 *
 ***************************************************************************/

#include <stdlib.h>
#include <grass/gis.h>
#include <grass/Rast.h>
#include <grass/glocale.h>
#include "local_proto.h"

static struct Cell_stats statf;

int get_cats(const char *name, const char *mapset)
{
    int fd;
    int row, nrows, ncols;
    CELL *cell;
    struct Cell_head cellhd;

    /* set the window to the cell header */
    if (Rast_get_cellhd(name, mapset, &cellhd) < 0)
	G_fatal_error(_("Cannot read header of raster map <%s> in <%s>"),
		      name, mapset);

    Rast_set_window(&cellhd);

    /* open the raster map */
    fd = Rast_open_cell_old(name, mapset);
    if (fd < 0)
	G_fatal_error(_("Cannot open cell file of raster map <%s> in <%s>"),
		      name, mapset);
    nrows = G_window_rows();
    ncols = G_window_cols();
    cell = Rast_allocate_cell_buf();
    Rast_init_cell_stats(&statf);

    /* read the raster map */
    G_verbose_message(_("Reading <%s> in <%s>"), name, mapset);
    for (row = 0; row < nrows; row++) {
	if (G_verbose() > G_verbose_std())
	    G_percent(row, nrows, 2);
	if (Rast_get_c_raster_row_nomask(fd, cell, row) < 0)
	    exit(EXIT_SUCCESS);
	Rast_update_cell_stats(cell, ncols, &statf);
    }
    /* done */
    if (G_verbose() > G_verbose_std())
	G_percent(row, nrows, 2);
    Rast_close_cell(fd);
    G_free(cell);
    Rast_rewind_cell_stats(&statf);

    return 0;
}

int next_cat(long *x)
{
    long count;
    CELL cat;

    if (Rast_next_cell_stat(&cat, &count, &statf)) {
	*x = cat;
	return 1;
    }

    return 0;
}
