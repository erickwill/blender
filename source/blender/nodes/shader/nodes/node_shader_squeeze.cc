/* SPDX-FileCopyrightText: 2005 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup shdnodes
 */

#include "node_shader_util.hh"

namespace blender::nodes::node_shader_squeeze_cc {

static void node_declare(NodeDeclarationBuilder &b)
{
  b.add_input<decl::Float>("Value").default_value(0.0f).min(-100.0f).max(100.0f);
  b.add_input<decl::Float>("Width").default_value(1.0f).min(-100.0f).max(100.0f);
  b.add_input<decl::Float>("Center").default_value(0.0f).min(-100.0f).max(100.0f);
  b.add_output<decl::Float>("Value");
}

static int gpu_shader_squeeze(GPUMaterial *mat,
                              bNode *node,
                              bNodeExecData * /*execdata*/,
                              GPUNodeStack *in,
                              GPUNodeStack *out)
{
  return GPU_stack_link(mat, node, "squeeze", in, out);
}

}  // namespace blender::nodes::node_shader_squeeze_cc

void register_node_type_sh_squeeze()
{
  namespace file_ns = blender::nodes::node_shader_squeeze_cc;

  static blender::bke::bNodeType ntype;

  sh_node_type_base(&ntype, "ShaderNodeSqueeze", SH_NODE_SQUEEZE);
  ntype.ui_name = "Squeeze Value (Legacy)";
  ntype.ui_description = "Deprecated";
  ntype.enum_name_legacy = "SQUEEZE";
  ntype.nclass = NODE_CLASS_CONVERTER;
  ntype.gather_link_search_ops = nullptr;
  ntype.declare = file_ns::node_declare;
  ntype.gpu_fn = file_ns::gpu_shader_squeeze;

  blender::bke::node_register_type(ntype);
}
