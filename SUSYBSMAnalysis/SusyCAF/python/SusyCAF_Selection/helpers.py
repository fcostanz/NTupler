import FWCore.ParameterSet.Config as cms
from copy import deepcopy
from PhysicsTools.PatAlgos.tools.helpers import listModules

def applySelection( process, collection, cut, selectorTemplate, filter=False):
    result = []
    modules = [collection] # in case we got a single module
    if "visit" in dir(collection): #in case we got a sequence or the like
        modules = listModules(collection)
    for module in modules:
        #make sure selectornames are unique
        i = 0
        while "selection%s%i"%(module.label(), i) in  process.filterNames().split(): i+= 1
        selectorLabel="selection%s%i"%(module.label(), i)
        #add selector NOTE: different names than "src" and "cut" not supported yet...
        # NOTE2: this supposes that the source for the nTupler is called "InputTag" which is consitantly the case. However some more intelligent things are possible, if needed.
        process.__setattr__(selectorLabel, selectorTemplate.clone(
                src = deepcopy(module.InputTag),
                cut = cut,
                filter = cms.bool(filter) #need cms.bool here because PAT _cfi.py do not include it :\
                )
                            )
        module.InputTag = selectorLabel
        result.append(process.__getattribute__(selectorLabel))
    return result
