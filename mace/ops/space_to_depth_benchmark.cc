// Copyright 2018 Xiaomi, Inc.  All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "mace/core/operator.h"
#include "mace/core/testing/test_benchmark.h"
#include "mace/ops/ops_test_util.h"

namespace mace {
namespace ops {
namespace test {

namespace {
template <DeviceType D, typename T>
void SpaceToDepth(
    int iters, int batch, int channels, int height, int width, int block_size) {
  mace::testing::StopTiming();

  OpsTestNet net;

  // Add input data
  net.AddRandomInput<D, float>("Input", {batch, height, width, channels});

  if (D == DeviceType::OPENCL) {
    BufferToImage<D, float>(&net, "Input", "InputImage",
                            kernels::BufferType::IN_OUT_CHANNEL);

    OpDefBuilder("SpaceToDepth", "SpaceToDepthBM")
        .Input("InputImage")
        .Output("Output")
        .AddIntArg("block_size", block_size)
        .Finalize(net.NewOperatorDef());
  } else {
    OpDefBuilder("SpaceToDepth", "SpaceToDepthBM")
        .Input("Input")
        .Output("Output")
        .Finalize(net.NewOperatorDef());
  }

  // Warm-up
  for (int i = 0; i < 5; ++i) {
    net.RunOp(D);
  }
  net.Sync();

  mace::testing::StartTiming();
  while (iters--) {
    net.RunOp(D);
  }
  net.Sync();
}
}  // namespace

#define BM_SPACE_TO_DEPTH_MACRO(N, C, H, W, G, TYPE, DEVICE)             \
  static void                                                            \
      BM_SPACE_TO_DEPTH_##N##_##C##_##H##_##W##_##G##_##TYPE##_##DEVICE( \
          int iters) {                                                   \
    const int64_t tot = static_cast<int64_t>(iters) * N * C * H * W;     \
    mace::testing::MaccProcessed(tot);                                   \
    mace::testing::BytesProcessed(tot *(sizeof(TYPE)));                  \
    SpaceToDepth<DEVICE, TYPE>(iters, N, C, H, W, G);                    \
  }                                                                      \
  BENCHMARK(BM_SPACE_TO_DEPTH_##N##_##C##_##H##_##W##_##G##_##TYPE##_##DEVICE)

#define BM_SPACE_TO_DEPTH(N, C, H, W, G)                 \
  BM_SPACE_TO_DEPTH_MACRO(N, C, H, W, G, float, CPU);    \
  BM_SPACE_TO_DEPTH_MACRO(N, C, H, W, G, float, OPENCL); \
  BM_SPACE_TO_DEPTH_MACRO(N, C, H, W, G, half, OPENCL);

BM_SPACE_TO_DEPTH(1, 64, 64, 64, 4);
BM_SPACE_TO_DEPTH(1, 64, 128, 128, 4);
BM_SPACE_TO_DEPTH(1, 64, 256, 256, 4);

}  // namespace test
}  // namespace ops
}  // namespace mace