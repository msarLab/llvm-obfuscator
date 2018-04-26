
#ifndef LLVM_OBFUSCATION_H
#define LLVM_OBFUSCATION_H

#include "llvm/Pass.h"

using namespace std;
using namespace llvm;

// Namespace
namespace llvm {
    ModulePass* createObfuscationPass();
}

#endif //LLVM_OBFUSCATION_H
