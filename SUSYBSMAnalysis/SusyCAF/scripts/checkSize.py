#!/usr/bin/python
####################################
import re,sys
####################################
def makeResultsList(inFileName) :
    inFile=open(inFileName)

    brName=""
    fileSize=""
    
    results=[]
    
    for line in inFile :
        fieldList=line.split()
        
        brLine=line.find('*Br')
        if brLine>-1 :
            brName=fieldList[2]
            brName=brName[1:]
            #print brName
        
        trLine=line.find('*Tree')
        if trLine>-1 :
            brName=fieldList[1]
            brName=brName[1:]
            #print brName

        enLine=line.find('*Entries')
        fs1Line=line.find('File Size')
        fs2Line=line.find('File  Size')
        if (enLine>-1 and (fs1Line>-1 or fs2Line>-1) ) : 
            fileSize = fieldList[11]
            entries = fieldList[2]
            #print fileSize,brName
            results.append( (long(fileSize),brName,long(entries)) )

    inFile.close()
    return results
####################################
def buildMatchDictionary(zoomOut = False) :
    d = {}

    #jets
    if zoomOut : 
        d["ak5Jets"]                =["^ak5"]
        d["ak7Jets"]                =["^ak7"]
        d["ic5Jets"]                =["^ic5"]
    else :
        #Calo
        d["ak5JetCalo"]            =["ak5Jet(?!Gen)(?!JPT)(?!PF)(?!MPT)"]
        d["ak7JetCalo"]            =["ak7Jet(?!Gen)(?!JPT)(?!PF)(?!MPT)"]
        d["ic5JetCalo"]            =["ic5Jet(?!Gen)(?!JPT)(?!PF)(?!MPT)"]
        
        d["ak5JetCaloGen"]         =["ak5JetGen(?!JPT)(?!PF)(?!MPT)"]
        d["ak7JetCaloGen"]         =["ak7JetGen(?!JPT)(?!PF)(?!MPT)"]
        d["ic5JetCaloGen"]         =["ic5JetGen(?!JPT)(?!PF)(?!MPT)"]
        
        #JPT
        d["ak5JetJPT"]             =["ak5JetJPT(?!Gen)(?!MPT)"]
        
        d["ak5JetJPTGen"]          =["ak5JetJPTGen(?!MPT)"]
    
        #PF
        d["ak5JetPF"]              =["ak5JetPF(?!2PAT)(?!Gen)(?!MPT)"]
        d["ak7JetPF"]              =["ak7JetPF(?!2PAT)(?!Gen)(?!MPT)"]
        
        d["ak5JetPFGen"]           =["ak5JetPF(?!2PAT)Gen"]
        d["ak7JetPFGen"]           =["ak7JetPF(?!2PAT)Gen"]
        
        #PF2PAT
        d["ak5JetPF2PAT"]          =["ak5JetPF2PAT(?!Gen)(?!MPT)"]
        d["ak7JetPF2PAT"]          =["ak7JetPF2PAT(?!Gen)(?!MPT)"]
        
        d["ak5JetPF2PATGen"]       =["ak5JetPF2PATGen"]
        d["ak7JetPF2PATGen"]       =["ak7JetPF2PATGen"]
        
        #MPT
        d["ak5MPT"]             =["ak5.*MPT.*"]
        d["ak7MPT"]             =["ak7.*MPT.*"]
        d["ic5MPT"]             =["ic5.*MPT.*"]
    #end else

    d["trigger"]               =["triggered","prescaled","hltHandleValid","L1HandleValid",
                                 "physicsDeclared", "parasiticTrigger","hltKey","hltL1Seeds",
                                 "physbits","techbits","^bx"]
    d["met"]                   =["met"]
    d["muons"]                 =["^muon"]
    d["photons"]               =["^photon"]
    d["electrons"]             =["^electron"]
    d["tau"]                   =["tau"]
    d["tracks"]                =["tracks"]
    d["event"]                 =["time","orbit","run","bunch","event","lumiSection","isRealData"]
    d["hcal noise summary"]    =["hns"]
    d["hcal noise rbx"]        =["hnr"]
    d["hbhe noise filter"]     =["hbheNoise"]
    d["rechit"]                =["rechit"]
    d["vertex"]                =["vertex"]
    d["calo towers"]           =["^CT"]
    d["dqm"]                   =["DQM"]
    d["dcs"]                   =["DCS"]
    d["beamspot"]              =["beamSpot"]
    d["coords"]                =["^fCoordinates"]
    d["tree"]                  =["tree"]
    d["gen particles"]         =["^gen(?!met)"]
    d["dead ecal"]             =["ecalDead"]
    d["dead hcal"]             =["hcalDead"]
    d["beam halo"]             =["beamHalo"]
    d["log error"]             =["logError"]
    
    return d
####################################
def histogramByCategory(results, zoomOut) :
    matchDictionary = buildMatchDictionary(zoomOut)

    #build count dictionary
    categoryCountDictionary={}
    for item in results :
        name=item[1]
        categoryCountDictionary[name]=0

    #initialize output list
    outputList=[]
    
    for key in matchDictionary :
        bytes=0L
        for item in results :
            fileSize = item[0]
            name = item[1]
            entries = item[2]
            for matchString in matchDictionary[key] :
                #if (name.find(matchString)>-1) :
                if (re.search(matchString,name)!=None) :
                    bytes+=fileSize
                    categoryCountDictionary[name]+=1

        outputList.append( (bytes, key, entries) )

    print "----------------------------------------------------------"
    print "non- or over-counted branches (or re-used names):"
    for key in categoryCountDictionary :
        if (categoryCountDictionary[key]!=1) :
            print key,categoryCountDictionary[key]

    return outputList
####################################
def printSorted(results) :
    results.sort()
    treeBytes = results[-1][0]
    treeEntries = results[-1][2]
    print "----------------------------------------------------------"
    print "sorted results:"
    for item in results :
        bytes = item[0]
        name = item[1]
        outString=name.ljust(30)
        outString+=": "
        outString+=str(bytes).rjust(15)
        outString+="  (%#6.2f"%( 100*(bytes+0.0)/treeBytes )
        outString+=" %)"
        outString+="  [%5.2f kB/event]"%( (bytes+0.0)/treeEntries/1024 )
        print outString
####################################
def printCategories(zoomOut) :
    dict=buildMatchDictionary(zoomOut)
    print "----------------------------------------------------------"    
    print "category definitions:"
    
    for item in sorted(dict.keys()) :
        outString=""
        outString+=item.ljust(30)
        outString+=str(dict[item])
        print outString
####################################
def fileName() :
    if len(sys.argv)<2 :
        print "usage:",sys.argv[0]," file.txt"
        sys.exit()
    else :
        return sys.argv[1]
####################################
def go(zoomOut, zoomIn) :
    results = makeResultsList(fileName())
    if zoomIn :
        printSorted(results)
    else :
        categorizedResults = histogramByCategory(results, zoomOut)
        printSorted(categorizedResults)
        printCategories(zoomOut)
####################################
go(zoomOut = False, zoomIn = False)
