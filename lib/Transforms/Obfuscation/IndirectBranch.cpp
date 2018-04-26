
#include "llvm/Transforms/Obfuscation/IndirectBranch.h"
#include "llvm/Transforms/Obfuscation/Utils.h"

bool IndirectBranch::runOnFunction(Function &F) {

    Function *tmp = &F;
    // Do we obfuscate
    if (toObfuscate(flag, tmp, "indir")) {
        if (!initBranch(F)) {
            llvm::errs() << "indirect branch failed.\n";
            return false;
        }
    }

    return true;
}

bool IndirectBranch::initBranch(Function &F) {

    Function *func = &F;
    indexmap.clear();
    vector<Constant *> BBs;
    unsigned long long i = 0;
    for (auto BB = func->begin(); BB != func->end(); BB++) {
        BasicBlock *BBPtr = &*BB;
        if (BBPtr != &(BBPtr->getParent()->getEntryBlock())) {
            indexmap[BBPtr] = i++;
            BBs.push_back(BlockAddress::get(BBPtr));
        }
    }
    ArrayType *AT =
            ArrayType::get(Type::getInt8PtrTy(func->getContext()), BBs.size());
    Constant *blockAddressArray =
            ConstantArray::get(AT, ArrayRef<Constant *>(BBs));
    std::string tableName = func->getName().str() + "_table";
    new GlobalVariable(*func->getParent(), AT, false, GlobalValue::LinkageTypes::PrivateLinkage,
                       blockAddressArray, tableName);
    indirectReplaceBranch(func);
    return true;
}

bool IndirectBranch::indirectReplaceBranch(Function *func) {

    std::vector<BranchInst *> BIs;
    for (inst_iterator I = inst_begin(func); I != inst_end(func); I++) {
        Instruction *Inst = &(*I);
        if (BranchInst *BI = dyn_cast<BranchInst>(Inst)) {
            BIs.push_back(BI);
        }
    }
    Value *zero =
            ConstantInt::get(Type::getInt32Ty(func->getParent()->getContext()), 0);
    for (BranchInst *BI : BIs) {
        IRBuilder<> IRB(BI);

        // We use the condition's evaluation result to generate the GEP
        // instruction  False evaluates to 0 while true evaluates to 1.  So here
        // we insert the false block first
        std::vector<BasicBlock *> BBs;
        for (int i = BI->getNumSuccessors() - 1; i >= 0; i--) {
            BBs.push_back(BI->getSuccessor(i));
        }
        GlobalVariable *LoadFrom = NULL;
        if (BI->isConditional() ||
            indexmap.find(BI->getSuccessor(0)) == indexmap.end()) {
            ArrayType *AT = ArrayType::get(Type::getInt8PtrTy(func->getContext()), BBs.size());
            std::vector<Constant *> blockAddresses;
            for (unsigned i = 0; i < BBs.size(); i++) {
                blockAddresses.push_back(BlockAddress::get(BBs[i]));
            }
            // Create a new GV
            Constant *BlockAddressArray =
                    ConstantArray::get(AT, ArrayRef<Constant *>(blockAddresses));
            LoadFrom = new GlobalVariable(*func->getParent(), AT, false,
                                          GlobalValue::LinkageTypes::PrivateLinkage,
                                          BlockAddressArray);
        } else {
            std::string tableName = func->getName().str() + "_table";
            LoadFrom = func->getParent()->getGlobalVariable(tableName, true);
        }
        Value *index = NULL;
        if (BI->isConditional()) {
            Value *condition = BI->getCondition();
            index = IRB.CreateZExt(
                    condition, Type::getInt32Ty(func->getParent()->getContext()));
        } else {
            index =
                    ConstantInt::get(Type::getInt32Ty(func->getParent()->getContext()),
                                     indexmap[BI->getSuccessor(0)]);
        }
        Value *GEP = IRB.CreateGEP(LoadFrom, {zero, index});
        LoadInst *LI = IRB.CreateLoad(GEP, "");
        IndirectBrInst *indirBr = IndirectBrInst::Create(LI, BBs.size());
        for (BasicBlock *BB : BBs) {
            indirBr->addDestination(BB);
        }
        ReplaceInstWithInst(BI, indirBr);
    }
    return true;
}