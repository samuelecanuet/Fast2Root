#ifndef DETECTOR_HH
#define DETECTOR_HH

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "TFile.h"
#include "TCanvas.h"
#include <TKey.h>
#include "TF1.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLine.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TRandom.h"
#include "TPaveText.h"
#include "TLegend.h"

#include "fasterac/fasterac.h"
#include "fasterac/fast_data.h"
#include "fasterac/qdc.h"
#include "fasterac/spectro.h"
#include "fasterac/utils.h"

#include "Messenger.hh"

using namespace std;

class Detector
{
    public: 
    Detector(string Name, int Lable, int Coder, TFile* file, double TOTAL_TIME);
    ~Detector();

    virtual void Fill(qdc_x1 value, int Time);
    virtual void Fill(qdc_x2 value, int Time);
    virtual void Fill(qdc_x3 value, int Time);
    virtual void Fill(qdc_x4 value, int Time);
    virtual void Fill(crrc4_spectro value, int Time);

    virtual void Write();
    int SetRangesFromFile();

    string GetName();
    int GetLabel();
    int GetCoder();
    int GetQDC_nb();
    bool GetDefaultRanges();

    string NAME;
    int LABEL;
    int CODER;
    tuple<int, int, int> Ranges;
    bool DEFAULT_RANGES = false;

    int BIN;
    double MIN;
    double MAX;


    TDirectory *DetectorDir;
    // TDirectory *Coderdir[4];
    // TH1D *Channel[4];
    // TH1D *Time[4];
    // TH2I *ChannelTime[4];
};

#endif