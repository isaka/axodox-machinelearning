#pragma once
#include "../../includes.h"
#include "MachineLearning/Executors/OnnxExecutor.h"
#include "OnnxEnvironment.h"
#include "OnnxModelSource.h"

namespace Axodox::MachineLearning::Sessions
{
  enum class OnnxExecutorType
  {
    Cpu = 0,
    Dml = 1
  };

  struct AXODOX_MACHINELEARNING_API OnnxSessionParameters
  {
    std::shared_ptr<OnnxEnvironment> Environment;
    std::shared_ptr<Executors::OnnxExecutor> Executor;
    std::shared_ptr<OnnxModelSource> ModelSource;

    static OnnxSessionParameters Create(const std::filesystem::path& path, OnnxExecutorType executorType);

    bool IsValid() const;
    explicit operator bool() const;
  };

  struct AXODOX_MACHINELEARNING_API OnnxSessionUnlock
  {
    void operator()(Ort::Session* value);
  };

  using OnnxSessionRef = Threading::locked_ptr<Ort::Session, OnnxSessionUnlock>;

  class AXODOX_MACHINELEARNING_API OnnxSessionContainer
  {
  public:
    OnnxSessionContainer(const OnnxSessionParameters& parameters);

    OnnxSessionRef Session();

    OnnxEnvironment* Environment() const;

  private:
    OnnxSessionParameters _parameters;
    Ort::Session _session;
    std::shared_mutex _mutex;
    Infrastructure::event_subscription _deviceResetSubscription;

    void EnsureSession();
    void OnDeviceReset(Executors::OnnxExecutor* executor);
  };
}