// Copyright (c) Meta Platforms, Inc. and affiliates.
// All rights reserved.
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#pragma once

#include <memory>
#include <vector>

#include <ATen/Generator.h>
#include <ATen/Tensor.h>

#include "fairseq2n/data/data_pipeline.h"
#include "fairseq2n/data/data_source.h"

namespace fairseq2n::detail {

class sample_data_source final : public data_source {
public:
    explicit
    sample_data_source(std::vector<data_pipeline> &&pipelines, std::vector<float32> &&weights);

    std::optional<data>
    next() override;

    void
    reset() override;

    void
    record_position(tape &t) const override;

    void
    reload_position(tape &t) override;

private:
    std::size_t
    random_pipeline_index();

    data
    next_in_pipeline(std::size_t pipeline_idx);

    bool
    are_all_done() noexcept;

private:
    std::vector<data_pipeline> pipelines_;
    at::Generator generator_;
    at::Tensor weights_;
    std::vector<data> buffer_{};
    std::vector<bool> is_epoch_done_;
    bool is_eod_ = false;
};

}  // namespace fairseq2::detail
