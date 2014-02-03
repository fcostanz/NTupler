#!/bin/zsh

export SCRAM_ARCH=slc5_amd64_gcc462
ini cmssw_cvmfs

cmsrel CMSSW_5_3_3_patch2
cd CMSSW_5_3_3_patch2/src/
cmsenv

git clone  https://github.com/fcostanz/NTupler.git
mv NTupler/* .
scram setup lhapdffull
touch $CMSSW_BASE/src/ElectroWeakAnalysis/Utilities/BuildFile.xml

scram b -j8
rehash

scram b -j8
rehash
cmsenv

cd SUSYBSMAnalysis/SusyCAF/test


cmsRun susydesy_cfg.py isData=1 output=SusyCAF_Tree_Data.root triggers=1 GlobalTag=FT_53_V6_AN3::All maxEvents=10 taus=0 jetCollections=ak5pf  files=/store/data/Run2012A/MuHad/AOD/13Jul2012-v1/00000/0009AB65-FFD0-E111-8622-003048FFD79C.root doTauReco=1

nafJobSplitterCfgMaker.sh Data /MuHad/Run2012A-13Jul2012-v1/AOD FT_53_V6C_AN3::All MuHad_Run2012A13Jul_cfg.py Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON_v2.txt
nafJobSplitterCfgMaker.sh Data /MuHad/Run2012B-13Jul2012-v1/AOD FT_53_V6C_AN3::All MuHad_Run2012B13Jul_cfg.py Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON_v2.txt

nafJobSplitter.pl -J -q 3 -M 2000 210 MuHad_Run2012A13Jul_cfg.py
nafJobSplitter.pl -J -q 3 -M 2000 420 MuHad_Run2012B13Jul_cfg.py

cmsRun susydesy_cfg.py isData=0 output=SusyCAF_Tree_T2bw.root triggers=1 GlobalTag=START53_V7G::All maxEvents=10 taus=0 jetCollections=ak5pf files=/store/mc/Summer12/SMS-T2bw_2J_mStop-100to475_mLSP-0to300_x-050_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola/AODSIM/START53_V7C_FSIM-v1/20000/001A48FF-2DB4-E211-A024-001EC9D282A8.root doTauReco=1 beamHaloVars=0 scan=T2bwFS53

nafJobSplitterCfgMaker.sh T2bw /SMS-T2bw_2J_mStop-100to475_mLSP-0to300_x-025_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola/Summer12-START53_V7C_FSIM-v1/AODSIM START53_V7G::All T2bw_mStop100To475mLSP0To300x0p25LepFilter_cfg.py
nafJobSplitterCfgMaker.sh T2bw /SMS-T2bw_2J_mStop-500to800_mLSP-0to700_x-025_TuneZ2star_8TeV-madgraph-tauola/Summer12-START53_V7C_FSIM-v1/AODSIM START53_V7G::All T2bw_mStop500To800mLSP0To700x0p25_cfg.py
nafJobSplitterCfgMaker.sh T2bw /SMS-T2bw_2J_mStop-100to475_mLSP-1_x-025_TuneZ2star_8TeV-madgraph-tauola/Summer12-START53_V7C_FSIM-v1/AODSIM START53_V7G::All T2bw_mStop100To475mLSP1x0p25_cfg.py
nafJobSplitterCfgMaker.sh T2bw /SMS-T2bw_2J_mStop-500to800_mLSP-1_x-025_TuneZ2star_8TeV-madgraph-tauola/Summer12-START53_V7C_FSIM-v1/AODSIM START53_V7G::All T2bw_mStop500To800mLSP1x0p25_cfg.py
nafJobSplitterCfgMaker.sh T2bw /SMS-T2bw_2J_mStop-100to475_mLSP-0to375_x-025_TuneZ2star_8TeV-madgraph-tauola/Summer12-START53_V7C_FSIM-v1/AODSIM START53_V7G::All T2bw_mStop100To475mLSP0To375x0p25_cfg.py
nafJobSplitterCfgMaker.sh T2bw /SMS-T2bw_2J_mStop-500to650_mLSP-150to300_x-025_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola/Summer12-START53_V7C_FSIM-v1/AODSIM START53_V7G::All T2bw_mStop500To650mLSP150To300x0p25LepFilter_cfg.py

nafJobSplitter.pl -J -q 3 -M 2000 300 T2bw_mStop100To475mLSP0To300x0p25LepFilter_cfg.py
#nafJobSplitter.pl -J -q 3 -M 2000 150 T2bw_mStop500To800mLSP0To700x0p25_cfg.py
#nafJobSplitter.pl -J -q 3 -M 2000 100 T2bw_mStop100To475mLSP1x0p25_cfg.py
#nafJobSplitter.pl -J -q 3 -M 2000  80 T2bw_mStop500To800mLSP1x0p25_cfg.py
#nafJobSplitter.pl -J -q 3 -M 2000  80 T2bw_mStop100To475mLSP0To375x0p25_cfg.py
#nafJobSplitter.pl -J -q 3 -M 2000  70 T2bw_mStop500To650mLSP150To300x0p25LepFilter_cfg.py