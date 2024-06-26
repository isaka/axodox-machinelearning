#pragma once
#include "MachineLearning/Tensor.h"

namespace Axodox::MachineLearning::Imaging::StableDiffusion::Schedulers
{
  enum class StableDiffusionSchedulerKind
  {
    EulerAncestral,
    DpmPlusPlus2M
  };

  struct AXODOX_MACHINELEARNING_API StableDiffusionSchedulerOptions
  {
    size_t TrainStepCount = 1000;
    size_t InferenceStepCount = 20;
    float BetaAtStart = 0.00085f;
    float BetaAtEnd = 0.012f;
    std::span<const float> BetasTrained;

    std::span<std::minstd_rand> Randoms;
  };

  class AXODOX_MACHINELEARNING_API StableDiffusionScheduler
  {
  public:
    StableDiffusionScheduler(const StableDiffusionSchedulerOptions& options);
    virtual ~StableDiffusionScheduler() = default;

    virtual Tensor ApplyStep(const Tensor& input, const Tensor& output, size_t step) = 0;

    static std::unique_ptr<StableDiffusionScheduler> Create(StableDiffusionSchedulerKind kind, const StableDiffusionSchedulerOptions& options);

    std::span<const float> Timesteps() const;
    std::span<const float> Sigmas() const;

  protected:
    std::vector<float> _timesteps;
    std::vector<float> _trainingSigmas, _sigmas;
    std::span<std::minstd_rand> _randoms;

    float SigmaToTime(float sigma) const;
  };
}