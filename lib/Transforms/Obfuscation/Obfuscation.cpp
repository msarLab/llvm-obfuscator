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
#include "llvm/Transforms/Obfuscation/Flattening.h"
#include "llvm/Transforms/Obfuscation/StringObfuscation.h"
#include "llvm/Transforms/Obfuscation/Substitution.h"
#include "llvm/Transforms/Obfuscation/BogusControlFlow.h"
#include "llvm/Transforms/Obfuscation/Split.h"
#include "llvm/Transforms/Obfuscation/IndirectBranch.h"
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

static cl::opt<bool> IndirectFlag("indir", cl::init(false),
                            cl::desc("Enable indirect branch."));



// End Obfuscator Options
namespace llvm {
    struct Obfuscation : public ModulePass {
        static char ID;
        Obfuscation() : ModulePass(ID) {}
        StringRef getPassName() const override {
            return StringRef("Obfuscation");
        }

        bool runOnModule(Module &M) override {

            SplitBasicBlock *split = new SplitBasicBlock(SplitFlag);
            for (Module::iterator I = M.begin(), E = M.end(); I != E; I++) {
                if (isa<Function>(&*I)) {
                    split->runOnFunction(*I);
                }
            }

            BogusControlFlow *bogus = new BogusControlFlow(BogusControlFlowFlag);
            for (Module::iterator I = M.begin(), E = M.end(); I != E; I++) {
                if (isa<Function>(&*I)) {
                    bogus->runOnFunction(*I);
                }
            }

            Flattening *flattening = new Flattening(FlatteningFlag);
            for (Module::iterator I = M.begin(), E = M.end(); I != E; I++) {
                if (isa<Function>(&*I)) {
                    flattening->runOnFunction(*I);
                }
            }

            IndirectBranch *idbranch = new IndirectBranch(IndirectFlag);
            for (Module::iterator I = M.begin(), E = M.end(); I != E; I++) {
                if (isa<Function>(&*I)) {
                    idbranch->runOnFunction(*I);
                }
            }

            Substitution *substitution = new Substitution(SubstitutionFlag);
            for (Module::iterator I = M.begin(), E = M.end(); I != E; I++) {
                if (isa<Function>(&*I)) {
                    substitution->runOnFunction(*I);
                }
            }

            StringObfuscation *strobf = new StringObfuscation(StringObfFlag);
            strobf->runOnModule(M);

            delete split;
            delete bogus;
            delete flattening;
            delete idbranch;
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
