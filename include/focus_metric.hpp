#pragma once

#include "config.hpp"

namespace focus_metric {

float random_score(const FrameView &frame);

float laplacian_score(const FrameView &frame);

}  // namespace focus_metric
