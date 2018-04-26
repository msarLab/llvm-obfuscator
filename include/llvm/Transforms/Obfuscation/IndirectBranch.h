
#ifndef LLVM_INDIRECTBRANCH_H
#define LLVM_INDIRECTBRANCH_H

#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace std;
using namespace llvm;

class IndirectBranch {
    bool flag;

public:
    IndirectBranch(bool flag) : flag(flag) { }

    bool runOnFunction(Function &F);
private:
    bool initBranch(Function &F);
    bool indirectReplaceBranch(Function *func);

    std::map<BasicBlock *, unsigned long long> indexmap;

};

#endif //LLVM_INDIRECTBRANCH_H
