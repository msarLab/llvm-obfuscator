
#ifndef LLVM_OBFUSCATION_H
#define LLVM_OBFUSCATION_H

#include "llvm/Transforms/Obfuscation/Flattening.h"
#include "llvm/Transforms/Obfuscation/StringObfuscation.h"
#include "llvm/Transforms/Obfuscation/Substitution.h"
#include "llvm/Transforms/Obfuscation/BogusControlFlow.h"
#include "llvm/Transforms/Obfuscation/Split.h"
using namespace std;
using namespace llvm;

// Namespace
namespace llvm {
    ModulePass* createObfuscationPass();
}

#endif //LLVM_OBFUSCATION_H
