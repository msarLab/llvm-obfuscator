//===- FlatteningIncludes.h - Flattening Obfuscation pass------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains includes and defines for the split basicblock pass
//
//===----------------------------------------------------------------------===//

#ifndef _SPLIT_INCLUDES_
#define _SPLIT_INCLUDES_


// LLVM include
#include "llvm/Pass.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Transforms/Utils/Local.h" // For DemoteRegToStack and DemotePHIToStack
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Transforms/Obfuscation/Utils.h"
#include "llvm/CryptoUtils.h"

class SplitBasicBlock {
    bool flag;
public:
    SplitBasicBlock(bool flag) : flag(flag) {}

    bool runOnFunction(Function &F);

private:
    void split(Function *f);

    bool containsPHI(BasicBlock *b);

    void shuffle(std::vector<int> &vec);
};

SplitBasicBlock *createSplitBasicBlock(bool flag) {
    return new SplitBasicBlock(flag);
}

#endif

