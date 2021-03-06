// Copyright 2019 The MACE Authors. All Rights Reserved.
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


#ifndef MACE_OPS_REF_CONV_2D_H_
#define MACE_OPS_REF_CONV_2D_H_

#include <vector>

#include "mace/core/ops/op_context.h"
#include "mace/core/tensor.h"
#include "mace/ops/common/conv_pool_2d_util.h"
#include "mace/ops/delegator/conv_2d.h"
#include "mace/public/mace.h"

namespace mace {
namespace ops {
namespace ref {

template<typename OUTPUT_TYPE>
class Conv2d : public delegator::Conv2d {
 public:
  explicit Conv2d(const delegator::Conv2dParam &param)
      : delegator::Conv2d(param) {}
  ~Conv2d() {}
  MaceStatus Compute(
      const OpContext *context,
      const Tensor *input,
      const Tensor *filter,
      Tensor *output) override;
};

template<>
class Conv2d<float> : public delegator::Conv2d {
 public:
  explicit Conv2d(const delegator::Conv2dParam &param)
      : delegator::Conv2d(param) {}
  ~Conv2d() {}

  MaceStatus Compute(
      const OpContext *context,
      const Tensor *input,
      const Tensor *filter,
      Tensor *output) override;
};

}  // namespace ref
}  // namespace ops
}  // namespace mace

#endif  // MACE_OPS_REF_CONV_2D_H_

