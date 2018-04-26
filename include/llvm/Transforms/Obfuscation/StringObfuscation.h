// LLVM include

#ifndef __STRING_OBFUSCATION__
#define __STRING_OBFUSCATION__

#include <string>
#include <strstream>

#include "llvm/Pass.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/CryptoUtils.h"

using namespace std;
using namespace llvm;

struct encVar {
public:
    GlobalVariable *var;
    char key;
};

class StringObfuscation {
public:
    bool flag;

    StringObfuscation(bool flag) : flag(flag) {}

    bool runOnModule(Module &M);

private:
    void addDecodeFunction(Module *mod, std::vector<encVar *> *gvars);
};

#endif