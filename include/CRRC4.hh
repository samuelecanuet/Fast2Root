#include "Detector.hh"
using namespace std;

class CRRC4 : public Detector
{
    public: 
    CRRC4(string Name, int Label, int Coder, TFile* file, double TOTAL_TIME);

    void Fill(crrc4_spectro value, int Time) override;
    void Write() override ;


    TH1D *Channel;
    TH1D *Time;
    TH1D *PileUp;
    TH1D *Saturated;

    TH2D *ChannelTime;
    TH2D *ChannelPileUp;
    TH2D *ChannelSaturated;
};