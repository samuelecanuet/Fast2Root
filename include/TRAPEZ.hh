#include "Detector.hh"
using namespace std;

class TRAPEZ : public Detector
{
    public: 
    TRAPEZ(string Name, int Label, int Coder, TFile* file, double TOTAL_TIME, string setupFile);

    void Fill(trapez_spectro value, double Time) override;
    void Fill(spectro_counter value, double Time) override;
    void Write() override ;
   
    TH1D *Channel;
    TH1D *Time;
    TH1D *PileUp;
    TH1D *Saturated;

    TH2D *ChannelTime;
    TH1D *ChannelPileUp;
    TH1D *ChannelSaturated;

    TH1D *Scaler;
    double last_scalercalc = 0;
    double last_scalersent = 0;
    double last_scalertrig = 0; 
    TH1D* DiffScalerTrigTime;
    TH1D* DiffScalerSentTime;
    TH1D* DiffScalerCalcTime;

    // string SetupFileName;

};