/* SPDX-FileCopyrightText: 2018 Blender Foundation
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Author: Sergey Sharybin. */

#include "opensubdiv_evaluator_capi.hh"

#include <cstddef>

OpenSubdiv_Evaluator *openSubdiv_createEvaluatorFromTopologyRefiner(
    blender::opensubdiv::TopologyRefinerImpl * /*topology_refiner*/,
    eOpenSubdivEvaluator /*evaluator_type*/,
    OpenSubdiv_EvaluatorCache * /*evaluator_cache*/)
{
  return NULL;
}

void openSubdiv_deleteEvaluator(OpenSubdiv_Evaluator * /*evaluator*/) {}

OpenSubdiv_EvaluatorCache *openSubdiv_createEvaluatorCache(eOpenSubdivEvaluator /*evaluator_type*/)
{
  return NULL;
}

void openSubdiv_deleteEvaluatorCache(OpenSubdiv_EvaluatorCache * /*evaluator_cache*/) {}

const char *openSubdiv_getGLSLPatchBasisSource()
{
  return NULL;
}
