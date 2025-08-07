#include "focus_metric.hpp"
#include <cstdlib>
#include <ctime>

namespace focus_metric {

float random_score(const FrameView &frame) {
  return static_cast<float>(rand() % 100) / 100.0f; // [0.0, 1.0]
}

float laplacian_score(const FrameView &frame) {
  // compute using openCV
  // - cv::Mat ...
  // - cv::Laplacian ...
  // - cv::meanStdDev ...
  return 0.0f;
}

} // namespace focus_metric
