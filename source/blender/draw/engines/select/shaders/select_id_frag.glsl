/* SPDX-FileCopyrightText: 2022-2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

void main()
{
  frag_color = floatBitsToUint(intBitsToFloat(select_id));
}
