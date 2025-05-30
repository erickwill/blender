/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup spview3d
 */

#include "BLI_math_vector.h"

#include "BKE_context.hh"

#include "WM_api.hh"

#include "view3d_intern.hh"

#include "view3d_navigate.hh" /* own include */

/* -------------------------------------------------------------------- */
/** \name View Center Cursor Operator
 * \{ */

static wmOperatorStatus viewcenter_cursor_exec(bContext *C, wmOperator *op)
{
  View3D *v3d = CTX_wm_view3d(C);
  RegionView3D *rv3d = CTX_wm_region_view3d(C);
  Scene *scene = CTX_data_scene(C);

  if (rv3d) {
    ARegion *region = CTX_wm_region(C);
    const int smooth_viewtx = WM_operator_smooth_viewtx_get(op);

    ED_view3d_smooth_view_force_finish(C, v3d, region);

    /* non camera center */
    float ofs_new[3];
    negate_v3_v3(ofs_new, scene->cursor.location);

    V3D_SmoothParams sview = {nullptr};
    sview.ofs = ofs_new;
    sview.undo_str = op->type->name;
    ED_view3d_smooth_view(C, v3d, region, smooth_viewtx, &sview);

    /* Smooth view does view-lock #RV3D_BOXVIEW copy. */
  }

  return OPERATOR_FINISHED;
}

void VIEW3D_OT_view_center_cursor(wmOperatorType *ot)
{
  /* identifiers */
  ot->name = "Center View to Cursor";
  ot->description = "Center the view so that the cursor is in the middle of the view";
  ot->idname = "VIEW3D_OT_view_center_cursor";

  /* API callbacks. */
  ot->exec = viewcenter_cursor_exec;
  ot->poll = view3d_location_poll;

  /* flags */
  ot->flag = 0;
}

/** \} */
