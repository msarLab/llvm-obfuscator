add_llvm_library(LLVMObfuscation
        IndirectBranch.cpp
        Obfuscation.cpp
  CryptoUtils.cpp
  Substitution.cpp
  BogusControlFlow.cpp
  Utils.cpp
  SplitBasicBlocks.cpp
  Flattening.cpp
  StringObfuscation.cpp
  )

add_dependencies(LLVMObfuscation intrinsics_gen)
