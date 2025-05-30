/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup freestyle
 */

#pragma once

#include "../BPy_UnaryFunction1DVec3f.h"

///////////////////////////////////////////////////////////////////////////////////////////

extern PyTypeObject Orientation3DF1D_Type;

#define BPy_Orientation3DF1D_Check(v) \
  (PyObject_IsInstance((PyObject *)v, (PyObject *)&Orientation3DF1D_Type))

/*---------------------------Python BPy_Orientation3DF1D structure definition----------*/
typedef struct {
  BPy_UnaryFunction1DVec3f py_uf1D_vec3f;
} BPy_Orientation3DF1D;

///////////////////////////////////////////////////////////////////////////////////////////
