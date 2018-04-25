//===- Obfuscation.cpp - Flattening Obfuscation pass------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the obfuscation pass
//
//===----------------------------------------------------------------------===//

#include "llvm/Transforms/Obfuscation/Obfuscation.h"
using namespace llvm;
using namespace std;
// Flags for obfuscation

static cl::opt<std::string> Seed("seed", cl::init(""),
                                 cl::desc("seed for the random"));

static cl::opt<bool> StringObfFlag("sobf", cl::init(false),
                               cl::desc("Enable the string obfuscation"));

// Flags for obfuscation
static cl::opt<bool> FlatteningFlag("fla", cl::init(false),
                                cl::desc("Enable the flattening pass"));

static cl::opt<bool> BogusControlFlowFlag("bcf", cl::init(false),
                                      cl::desc("Enable bogus control flow"));

static cl::opt<bool> SubstitutionFlag("sub", cl::init(false),
                                  cl::desc("Enable instruction substitutions"));

static cl::opt<std::string> AesSeed("aesSeed", cl::init(""),
                                    cl::desc("seed for the AES-CTR PRNG"));

static cl::opt<bool> SplitFlag("split", cl::init(false),
                           cl::desc("Enable basic block splitting"));



// End Obfuscator Options
namespace llvm {
    struct Obfuscation : public ModulePass {
        static char ID;
        Obfuscation() : ModulePass(ID) {}
        StringRef getPassName() const override {
            return StringRef("Obfuscation");
        }

        bool runOnModule(Module &M) override {

            SplitBasicBlock *split = createSplitBasicBlock(SplitFlag);
            for (Module::iterator I = M.begin(), E = M.end(); I != E; I++) {
                if (isa<Function>(&*I)) {
                    split->runOnFunction(*I);
                }
            }

            BogusControlFlow *bogus = createBogus(BogusControlFlowFlag);
            for (Module::iterator I = M.begin(), E = M.end(); I != E; I++) {
                if (isa<Function>(&*I)) {
                    bogus->runOnFunction(*I);
                }
            }

            Flattening *flattening = createFlattening(FlatteningFlag);
            for (Module::iterator I = M.begin(), E = M.end(); I != E; I++) {
                if (isa<Function>(&*I)) {
                    flattening->runOnFunction(*I);
                }
            }

            Substitution *substitution = createSubstitution(SubstitutionFlag);
            for (Module::iterator I = M.begin(), E = M.end(); I != E; I++) {
                if (isa<Function>(&*I)) {
                    substitution->runOnFunction(*I);
                }
            }

            StringObfuscation *strobf = createStringObfuscation(StringObfFlag);
            strobf->runOnModule(M);

            delete split;
            delete bogus;
            delete flattening;
            delete substitution;
            delete strobf;
            return true;
        }
    };
    ModulePass *createObfuscationPass() {
        // secure pseudo-random generator
        if(!AesSeed.empty()) {
            if(!llvm::cryptoutils->prng_seed(AesSeed.c_str())) {
                llvm::errs() << "aes seed set failed.\n";
            }
        }
        if(!Seed.empty()) {
            llvm::cryptoutils->prng_seed(Seed.c_str());
        }
        return new Obfuscation();
    }
} // namespace llvm
char Obfuscation::ID = 0;
