/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup DNA
 */

#pragma once

/* clang-format off */

#define _DNA_DEFAULT_bArmature \
  { \
    .deformflag = ARM_DEF_VGROUP | ARM_DEF_ENVELOPE, \
    .flag = ARM_COL_CUSTOM,  /* custom bone-group colors */ \
    .layer = 1, \
    .drawtype = ARM_DRAW_TYPE_OCTA, \
  }

#define _DNA_DEFAULT_Bone \
  { \
    .drawtype = ARM_DRAW_TYPE_ARMATURE_DEFINED, \
  }

/* clang-format on */
