#!/usr/bin/env python

import distutils
import distutils.fancy_getopt
import os
import sys
import string
import re


#---Settings----#000000#FFFFFF--------------------------------------------------

class Color:
  "ANSI escape display sequences"
  info          = "\033[1;34m"
  hilight       = "\033[31m"
  alternate     = "\033[32m"
  extra         = "\033[33m"
  backlight     = "\033[43m"
  underline     = "\033[4m"
  lessemphasis  = "\033[30m"
  deemphasis    = "\033[1;30m"
  none          = "\033[0m"

class Error:
  "Exit codes"
  none          = 0
  usage         = 1
  argument      = 2
  execution     = 3
  logic         = 4

class Settings:
  "Settings for execution etc."

  blurb         = """
Usage:    %program% [options] [tag1=pattern1...] <data-file-paths...>
Author:   Sue Ann Koay (sakoay@cern.ch)

You can provide more than one data file (if they have the same tree name) in case some branches are
different and you want to dump the union of all branches over all trees.

By default the list of branches is dumped in the LEAF form for use in TTREE_FOREACH_ENTRY. You can
also rename the branch collections by specifying some number of "tag=pattern" rules in the command
line. These can occur before, after, or even in-between the data file paths, since they are
distinguished from the paths by syntax (paths are not allowed to have "=" in this script).

For example, the command:
 %program% jet=ak5Jet patTree.root
 
should generate output [partially] like:
  trigger_t           * LEAF        (triggered             )
  Points              * LEAF        (vertexPosition        )
  std::vector<double> * RENAMED_LEAF(jetCorrFactorPat      , "ak5JetCorrFactorPat"      )
  LorentzVs           * RENAMED_LEAF(jetCorrectedP4Pat     , "ak5JetCorrectedP4Pat"     )
  std::vector<double> * RENAMED_LEAF(jetEmEnergyFractionPat, "ak5JetEmEnergyFractionPat")


Be careful that the tagging is done on a first-come, first-match basis, i.e. the order in which
you provide the "tag=pattern" rules is important.

By the way, the "pattern" in "tag=pattern" is interpreted as a regular expressions, in case you
need this level of control. "tag" will become the collection name, so be careful that you provide
a C-compliant identifier. If you're feeling brave, try "jet\1=ak5Jet(.+)Pat"!

Command-line options can be:
"""
  variables     = distutils.fancy_getopt.FancyGetopt([
                    ("help",                  "h",  "Print this informational message."),
                    ("tree-path=",            "t",  "Path and name of tree to dump the branches of. Default %tree-path%."),
                    ("match-pattern=",        "m",  "Regular expression pattern to match the list of branches to print. Default everything."),
                    ("veto-pattern=",         "v",  "Regular expression pattern to veto the list of branches to print. Veto is applied after the branch has passed match-pattern. Default nothing."),
                    ("suppress-std-prefix",   "s",  "If set, suppresses the 'std::' prefix in front of std types. Default %suppress-std-prefix%."),
                  ])
  options       = { "help"                  : False,
                    "tree-path"             : "/susyTree/tree",
                    "match-pattern"         : None,
                    "veto-pattern"          : None,
                    "suppress-std-prefix"   : False,
                  }
  usage         = None

  matchTagRule  = re.compile("^([^=]+)=(.+)$")
  # Hard-coded list of typedefs... easiest this way
  rootTypes     = [ ("Double32_t","double"), ("Long64_t","long long") ]
  typedefs      = { "LorentzV"   : "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >"
                  , "Vector"     : "ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag>"
                  , "Point"      : "ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag>"
                  , "LorentzVs"  : "std::vector<LorentzV>"
                  , "Vectors"    : "std::vector<Vector>"
                  , "Points"     : "std::vector<Point>"
                  , "trigger_t"  : "std::map<std::string,bool>"
                  }
  # Hard-coded translations for ROOT primitive leaf types
  unsignedTypes = [("D","double"), ("F","float")]
  signedTypes   = [("I","int"), ("B","char"), ("S","short"), ("L","long"), ("O","bool")]
  # Unless you want to have a "using namespace std", we probably need these
  stdTypes      = re.compile(r"\b(vector|map|set|pair|string)\b")
  # Some cleanup requirements
  getExtraSpace = re.compile("(?<=[^>])[ \t]+(?=>)")


#---Functions---#000000#FFFFFF--------------------------------------------------

def convertSetting(setting, convertor, errorMessage):
  """
  Convert the given setting using the form produced by convertor. ValueError's
  are caught and errorMessage is printed, then the application exits. errorMessage
  is a format string taking one string argument, the problematic value.
  """

  try:
    Settings.options[setting] = convertor(Settings.options[setting])
  except ValueError:
    print Color.hilight + (errorMessage % str(Settings.options[setting])) + Color.none
    sys.exit(Error.argument)

  return


def parseArgs(args):
  "Parses command-line arguments into options and variables"

  Settings.scriptDir  = os.path.dirname(os.path.abspath(args[0]))
  Settings.blurb      = Settings.blurb.replace("%program%", os.path.basename(args[0]))
  Settings.usage      = string.join(Settings.variables.generate_help(Settings.blurb), "\n")
  for (variable, default) in Settings.options.iteritems():
    if type(default) == type(""):
      Settings.options[variable]  = default.replace("%script-dir%", Settings.scriptDir)
  for (variable, default) in Settings.options.iteritems():
    Settings.usage    = re.compile("%" + variable.replace("-", "-\s*") + "%") \
                          .sub(str(default), Settings.usage)

  try:
    (arguments, options)  = Settings.variables.getopt(args[1:])
  except distutils.errors.DistutilsArgError, details:
    print "Error in command-line:", details
    print Settings.usage
    sys.exit(Error.usage)

  # Get the dictionary of option -> value
  Settings.options.update(dict(Settings.variables.get_option_order()))

  # Special options
  if Settings.options["match-pattern"]:
    convertSetting("match-pattern", re.compile, "%s must be a valid regular expression.")
  if Settings.options["veto-pattern"]:
    convertSetting("veto-pattern", re.compile, "%s must be a valid regular expression.")


  if Settings.options["help"] or len(arguments) < 1:
    print Settings.usage
    print
    sys.exit(Error.none)
    
    
  # Parse arguments to distinguish the list of input files and the list of collection tags
  filePaths           = []
  patternTags         = []
  for argument in arguments:
    tagRule           = Settings.matchTagRule.search(argument)
    if tagRule:
      (tag, pattern)  = tagRule.groups()
      if pattern in patternTags:
        print Color.hilight + "ERROR:  Pattern '" + pattern + "' has already been assigned a tag '" + patternTags[pattern] + "'. Are you sure you haven't made a mistake?", Color.none
        sys.exit(Error.argument)
      try:
        patternTags.append((re.compile(pattern), tag))
      except sre_constants.error:
        print Color.hilight + "WARNING:  Pattern '" + pattern + "' is not a valid regular expression. Will interpret as plain text.", Color.none
        patternTags.append((pattern, tag))
    else:             filePaths.append(argument)

  return (filePaths, patternTags)


def compareList(xList, yList):
  for (x, y) in zip(xList, yList):
    comparison    = cmp(x, y)
    if comparison != 0:   return comparison
  return 0



#---Main Execution Point---#D50000#FFFF80---------------------------------------

if __name__ == '__main__':

  (filePaths, patternTags)  = parseArgs(sys.argv)
  
  #.. Load the entire chain in order to get all branches ......................
  sys.argv.append('-b-')     # Batch mode for ROOT
  from ROOT import TChain, TIter
  dataTree                  = TChain(os.path.basename(Settings.options["tree-path"]))
  for path in filePaths:
    dataTree.Add(path + Settings.options["tree-path"])
  

  #.. Process list of branches into plain and tagged collections ..............
  maxTypeLen                = 1
  maxCollLen                = 1
  maxNameLen                = 1
  maxTypeNameLen            = 1
  maxTypeDefLen             = 1
  plainColls                = []
  taggedColls               = []
  usedTypeDefs              = set()
  branches                  = dataTree.GetListOfBranches()
  for branch in branches:
    branchName              = branch.GetName()
    if Settings.options["match-pattern"] and not Settings.options["match-pattern"].search(branchName):  continue
    if Settings.options["veto-pattern"] and Settings.options["veto-pattern"].search(branchName):        continue
    
    branchTag               = None
    for (pattern, tag) in patternTags:
      if type(pattern) == type(""):
        if pattern in branchName:
          branchTag         = branchName.replace(pattern, tag)
          break
      elif pattern.search(branchName):
        branchTag           = pattern.sub(tag, branchName)
        break
    
    # Needs a bit of detective work to find the class name
    branchType              = branch.GetClassName()
    branchPointer           = "*"
    if branchType == "":
      branchPointer         = " "
      aLeaf                 = branch.GetListOfLeaves().At(0)
      leafType              = aLeaf.ClassName()[-1]
      for (postfix, typeName) in Settings.unsignedTypes:
        if leafType == postfix:
          branchType        = typeName
          break
      if branchType == "":
        for (postfix, typeName) in Settings.signedTypes:
          if leafType == postfix:
            if aLeaf.IsUnsigned():    branchType  = "unsigned " + typeName
            else:                     branchType  = typeName
            break
      if branchType == "":
        print Color.hilight + "ERROR:  Unsupported leaf type '" + aLeaf.ClassName() + "'. Please complain to the author.", Color.none
        sys.exit(Error.logic)
        
    # Prettify those long type names a bit, and add the std:: prefix where necessary
    for (typeName, typeDef) in Settings.rootTypes:
      if typeName in branchType:      branchType  = branchType.replace(typeName, typeDef)
    for (typeName, typeDef) in Settings.typedefs.iteritems():
      strippedTypeDef       = typeDef.replace("std::", "")
      if strippedTypeDef in branchType:
        branchType          = Settings.getExtraSpace.sub("", branchType.replace(strippedTypeDef, typeName))
        usedTypeDefs.add(typeName)
        maxTypeNameLen      = max(maxTypeNameLen, len(typeName))
        maxTypeDefLen       = max(maxTypeDefLen , len(typeDef))
    if not Settings.options["suppress-std-prefix"]:
      branchType            = Settings.stdTypes.sub(r"std::\1", branchType)
        
    # Finally, store info and length to display them all nicely in a row
    maxTypeLen              = max(maxTypeLen, len(branchType))
    if branchTag:
      maxCollLen            = max(maxCollLen, len(branchTag))
      maxNameLen            = max(maxNameLen, len(branchName))
      taggedColls.append((branchTag, branchName, branchType, branchPointer))
    else:
      maxCollLen            = max(maxCollLen, len(branchName))
      plainColls.append((branchName, branchType, branchPointer))


  #.. Sort and display ........................................................
  plainColls .sort(compareList)
  taggedColls.sort(compareList)
  
  print
  print
  for typeName in usedTypeDefs:
    print "typedef %-*s %-*s;" % (maxTypeDefLen, Settings.typedefs[typeName], maxTypeNameLen, typeName)
  print
  
  maxDeclarationLen         = 4                       # LEAF
  if len(taggedColls) > 0:  maxDeclarationLen = 12    # RENAMED_LEAF
  
  print "TTREE_FOREACH_ENTRY(getChain(),"
  for (branchName, branchType, branchPointer) in plainColls:
    print "  %-*s %s %-*s(%-*s)" % (maxTypeLen, branchType, branchPointer, maxDeclarationLen, "LEAF", maxCollLen, branchName)
  for (branchTag, branchName, branchType, branchPointer) in taggedColls:
    print "  %-*s %s %-*s(%-*s, %-*s)" % (maxTypeLen, branchType, branchPointer, maxDeclarationLen, "RENAMED_LEAF", maxCollLen, branchTag, maxNameLen+2, '"'+branchName+'"')
  print ")"
  print
  print

