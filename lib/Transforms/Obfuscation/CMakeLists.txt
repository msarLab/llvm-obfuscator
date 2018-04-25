add_llvm_library(LLVMObfuscation
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
