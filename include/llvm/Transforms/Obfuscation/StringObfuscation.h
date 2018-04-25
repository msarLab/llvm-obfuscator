// LLVM include
#include "llvm/Pass.h"


class StringObfuscation {
public:
    bool flag;

    StringObfuscation(bool flag) : flag(flag) {}

    bool runOnModule(Module &M);

private:
    void addDecodeFunction(Module *mod, std::vector<encVar *> *gvars);
};

StringObfuscation *createStringObfuscation(bool flag) {
    return new StringObfuscation(flag);
}

