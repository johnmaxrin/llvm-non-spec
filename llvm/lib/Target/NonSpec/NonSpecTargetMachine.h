#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include "llvm/Target/TargetOptions.h"

#include <optional>
#include <utility>

namespace llvm {
class NonSpecTargetMachine : public CodeGenTargetMachineImpl {

public:
  NonSpecTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                       StringRef FS, const TargetOptions &Options,
                       std::optional<Reloc::Model> RM,
                       std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                       bool JIT);

  ~NonSpecTargetMachine() override;
};
} // namespace llvm
