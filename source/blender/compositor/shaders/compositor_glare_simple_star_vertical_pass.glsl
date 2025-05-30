/* SPDX-FileCopyrightText: 2022 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

#include "gpu_shader_compositor_texture_utilities.glsl"

void main()
{
  int height = imageSize(vertical_img).y;

  /* For each iteration, apply a causal filter followed by a non causal filters along the column
   * mapped to the current thread invocation. */
  for (int i = 0; i < iterations; i++) {
    /* Causal Pass:
     * Sequentially apply a causal filter running from bottom to top by mixing the value of the
     * pixel in the column with the average value of the previous output and next input in the same
     * column. */
    for (int y = 0; y < height; y++) {
      int2 texel = int2(gl_GlobalInvocationID.x, y);
      float4 previous_output = imageLoad(vertical_img, texel - int2(0, i));
      float4 current_input = imageLoad(vertical_img, texel);
      float4 next_input = imageLoad(vertical_img, texel + int2(0, i));

      float4 neighbor_average = (previous_output + next_input) / 2.0f;
      float4 causal_output = mix(current_input, neighbor_average, fade_factor);
      imageStore(vertical_img, texel, causal_output);
      imageFence(vertical_img);
    }

    /* Non Causal Pass:
     * Sequentially apply a non causal filter running from top to bottom by mixing the value of the
     * pixel in the column with the average value of the previous output and next input in the same
     * column. */
    for (int y = height - 1; y >= 0; y--) {
      int2 texel = int2(gl_GlobalInvocationID.x, y);
      float4 previous_output = imageLoad(vertical_img, texel + int2(0, i));
      float4 current_input = imageLoad(vertical_img, texel);
      float4 next_input = imageLoad(vertical_img, texel - int2(0, i));

      float4 neighbor_average = (previous_output + next_input) / 2.0f;
      float4 non_causal_output = mix(current_input, neighbor_average, fade_factor);
      imageStore(vertical_img, texel, non_causal_output);
      imageFence(vertical_img);
    }
  }

  /* For each pixel in the column mapped to the current invocation thread, add the result of the
   * horizontal pass to the vertical pass. */
  for (int y = 0; y < height; y++) {
    int2 texel = int2(gl_GlobalInvocationID.x, y);
    float4 horizontal = texture_load(horizontal_tx, texel);
    float4 vertical = imageLoad(vertical_img, texel);
    float4 combined = horizontal + vertical;
    imageStore(vertical_img, texel, float4(combined.rgb, 1.0f));
  }
}
