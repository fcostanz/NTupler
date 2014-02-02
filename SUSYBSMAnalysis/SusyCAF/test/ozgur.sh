#!/bin/zsh
cmsRun susydesy_cfg.py isData=0 output=SusyCAF_Tree_T2bw.root triggers=1 GlobalTag=START53_V7G::All maxEvents=10 taus=0 jetCollections=ak5pf files=/store/mc/Summer12/SMS-T2bw_2J_mStop-100to475_mLSP-0to300_x-050_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola/AODSIM/START53_V7C_FSIM-v1/20000/001A48FF-2DB4-E211-A024-001EC9D282A8.root doTauReco=1 beamHaloVars=0 scan=T2bwFS53


nafJobSplitter.pl -J -q 3 -M 2000 300 T2bw_mStop100To475mLSP0To300x0p25LepFilter_cfg.py
nafJobSplitter.pl -J -q 3 -M 2000 150 T2bw_mStop500To800mLSP0To700x0p25_cfg.py
nafJobSplitter.pl -J -q 3 -M 2000 100 T2bw_mStop100To475mLSP1x0p25_cfg.py
nafJobSplitter.pl -J -q 3 -M 2000  80 T2bw_mStop500To800mLSP1x0p25_cfg.py
nafJobSplitter.pl -J -q 3 -M 2000  80 T2bw_mStop100To475mLSP0To375x0p25_cfg.py
nafJobSplitter.pl -J -q 3 -M 2000  70 T2bw_mStop500To650mLSP150To300x0p25LepFilter_cfg.py





nafJobSplitterCfgMaker.sh T2bw /SMS-T2bw_2J_mStop-100to475_mLSP-0to300_x-025_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola/Summer12-START53_V7C_FSIM-v1/AODSIM START53_V7G::All T2bw_mStop100To475mLSP0To300x0p25LepFilter_cfg.py

nafJobSplitterCfgMaker.sh T2bw /SMS-T2bw_2J_mStop-500to800_mLSP-0to700_x-025_TuneZ2star_8TeV-madgraph-tauola/Summer12-START53_V7C_FSIM-v1/AODSIM START53_V7G::All T2bw_mStop500To800mLSP0To700x0p25_cfg.py

nafJobSplitterCfgMaker.sh T2bw /SMS-T2bw_2J_mStop-100to475_mLSP-1_x-025_TuneZ2star_8TeV-madgraph-tauola/Summer12-START53_V7C_FSIM-v1/AODSIM START53_V7G::All T2bw_mStop100To475mLSP1x0p25_cfg.py

nafJobSplitterCfgMaker.sh T2bw /SMS-T2bw_2J_mStop-500to800_mLSP-1_x-025_TuneZ2star_8TeV-madgraph-tauola/Summer12-START53_V7C_FSIM-v1/AODSIM START53_V7G::All T2bw_mStop500To800mLSP1x0p25_cfg.py

nafJobSplitterCfgMaker.sh T2bw /SMS-T2bw_2J_mStop-100to475_mLSP-0to375_x-025_TuneZ2star_8TeV-madgraph-tauola/Summer12-START53_V7C_FSIM-v1/AODSIM START53_V7G::All T2bw_mStop100To475mLSP0To375x0p25_cfg.py

nafJobSplitterCfgMaker.sh T2bw /SMS-T2bw_2J_mStop-500to650_mLSP-150to300_x-025_LeptonFilter_TuneZ2star_8TeV-madgraph-tauola/Summer12-START53_V7C_FSIM-v1/AODSIM START53_V7G::All T2bw_mStop500To650mLSP150To300x0p25LepFilter_cfg.py





