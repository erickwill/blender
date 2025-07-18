/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup pythonintern
 *
 * This file is so Python can define widget-group's that C can call into.
 * The generic callback functions for Python widget-group are defines in
 * 'rna_wm.cc', some calling into functions here to do python specific
 * functionality.
 *
 * \note This follows `bpy_operator_wrap.cc` very closely.
 * Keep in sync unless there is good reason not to!
 */

#include <Python.h>

#include "WM_types.hh"

#include "RNA_access.hh"
#include "RNA_define.hh"
#include "RNA_enum_types.hh"

#include "bpy_gizmo_wrap.hh" /* own include */
#include "bpy_intern_string.hh"
#include "bpy_rna.hh"

#include "../generic/py_capi_rna.hh"
#include "../generic/python_compat.hh" /* IWYU pragma: keep. */

/* we may want to add, but not now */

/* -------------------------------------------------------------------- */
/** \name Gizmo
 * \{ */

static bool bpy_gizmotype_target_property_def(wmGizmoType *gzt, PyObject *item)
{
  /* NOTE: names based on `rna_rna.cc`. */
  PyObject *empty_tuple = PyTuple_New(0);

  struct {
    char *id;
    BPy_EnumProperty_Parse type_enum;
    int array_length;
  } params{};
  params.id = nullptr; /* not optional */
  params.type_enum.items = rna_enum_property_type_items;
  params.type_enum.value = PROP_FLOAT;
  params.array_length = 1;

  static const char *const _keywords[] = {"id", "type", "array_length", nullptr};
  static _PyArg_Parser _parser = {
    PY_ARG_PARSER_HEAD_COMPAT()
      "|$" /* Optional keyword only arguments. */
      "s"  /* `id` */
      "O&" /* `type` */
      "i"  /* `array_length` */
      ":register_class",
      _keywords,
      nullptr,
  };
  if (!_PyArg_ParseTupleAndKeywordsFast(empty_tuple,
                                        item,
                                        &_parser,
                                        &params.id,
                                        pyrna_enum_value_parse_string,
                                        &params.type_enum,
                                        &params.array_length))
  {
    goto fail;
  }

  if (params.id == nullptr) {
    PyErr_SetString(PyExc_ValueError, "'id' argument not given");
    goto fail;
  }

  if ((params.array_length < 1) || (params.array_length > RNA_MAX_ARRAY_LENGTH)) {
    PyErr_SetString(PyExc_ValueError, "'array_length' out of range");
    goto fail;
  }

  WM_gizmotype_target_property_def(gzt, params.id, params.type_enum.value, params.array_length);
  Py_DECREF(empty_tuple);
  return true;

fail:
  Py_DECREF(empty_tuple);
  return false;
}

static void gizmo_properties_init(wmGizmoType *gzt)
{
  PyTypeObject *py_class = static_cast<PyTypeObject *>(gzt->rna_ext.data);
  RNA_struct_blender_type_set(gzt->rna_ext.srna, gzt);

  /* only call this so pyrna_deferred_register_class gives a useful error
   * WM_operatortype_append_ptr will call RNA_def_struct_identifier
   * later */
  RNA_def_struct_identifier_no_struct_map(gzt->srna, gzt->idname);

  if (pyrna_deferred_register_class(gzt->srna, py_class) != 0) {
    PyErr_Print(); /* failed to register operator props */
  }

  /* Extract target property definitions from 'bl_target_properties' */
  {
    /* NOTE(@ideasman42): Picky developers will notice that `bl_targets`
     * won't work with inheritance get direct from the dict to avoid
     * raising a load of attribute errors (yes this isn't ideal). */
    PyObject *py_class_dict = py_class->tp_dict;
    PyObject *bl_target_properties = PyDict_GetItem(py_class_dict,
                                                    bpy_intern_str_bl_target_properties);

    /* Some widgets may only exist to activate operators. */
    if (bl_target_properties != nullptr) {
      PyObject *bl_target_properties_fast;
      if (!(bl_target_properties_fast = PySequence_Fast(bl_target_properties,
                                                        "bl_target_properties sequence")))
      {
        /* PySequence_Fast sets the error */
        PyErr_Print();
        return;
      }

      const uint items_len = PySequence_Fast_GET_SIZE(bl_target_properties_fast);
      PyObject **items = PySequence_Fast_ITEMS(bl_target_properties_fast);

      for (uint i = 0; i < items_len; i++) {
        if (!bpy_gizmotype_target_property_def(gzt, items[i])) {
          PyErr_Print();
          break;
        }
      }

      Py_DECREF(bl_target_properties_fast);
    }
  }
}

void BPY_RNA_gizmo_wrapper(wmGizmoType *gzt, void *userdata)
{
  /* take care not to overwrite anything set in
   * #WM_gizmomaptype_group_link_ptr before `opfunc()` is called. */
  StructRNA *srna = gzt->srna;
  *gzt = *((wmGizmoType *)userdata);
  gzt->srna = srna; /* restore */

/* don't do translations here yet */
#if 0
  /* Use i18n context from rna_ext.srna if possible (py gizmo-groups). */
  if (gt->rna_ext.srna) {
    RNA_def_struct_translation_context(gt->srna, RNA_struct_translation_context(gt->rna_ext.srna));
  }
#endif

  gzt->struct_size = sizeof(wmGizmo);

  gizmo_properties_init(gzt);
}

/** \} */

/* -------------------------------------------------------------------- */
/** \name Gizmo Group
 * \{ */

static void gizmogroup_properties_init(wmGizmoGroupType *gzgt)
{
  PyTypeObject *py_class = static_cast<PyTypeObject *>(gzgt->rna_ext.data);
  RNA_struct_blender_type_set(gzgt->rna_ext.srna, gzgt);

  /* only call this so pyrna_deferred_register_class gives a useful error
   * WM_operatortype_append_ptr will call RNA_def_struct_identifier
   * later */
  RNA_def_struct_identifier_no_struct_map(gzgt->srna, gzgt->idname);

  if (pyrna_deferred_register_class(gzgt->srna, py_class) != 0) {
    PyErr_Print(); /* failed to register operator props */
  }
}

void BPY_RNA_gizmogroup_wrapper(wmGizmoGroupType *gzgt, void *userdata)
{
  /* take care not to overwrite anything set in
   * WM_gizmomaptype_group_link_ptr before opfunc() is called */
  StructRNA *srna = gzgt->srna;
  *gzgt = *((wmGizmoGroupType *)userdata);
  gzgt->srna = srna; /* restore */

/* don't do translations here yet */
#if 0
  /* Use i18n context from rna_ext.srna if possible (py gizmo-groups). */
  if (gzgt->rna_ext.srna) {
    RNA_def_struct_translation_context(gzgt->srna,
                                       RNA_struct_translation_context(gzgt->rna_ext.srna));
  }
#endif

  gizmogroup_properties_init(gzgt);
}

/** \} */
