#include "Detector.hh"
using namespace std;

class QDC : public Detector
{
    public: 
    QDC(string Name, int Label, int Coder, TFile* file, double TOTAL_TIME, string setupFile);

    void Fill(qdc_x1 value, double Time) override;
    void Fill(qdc_x2 value, double Time) override;
    void Fill(qdc_x3 value, double Time) override;
    void Fill(qdc_x4 value, double Time) override;
    void Fill(qdc_counter value, double Time) override;
    void Write() override ;

    int QDC_NB;
    TDirectory *Coderdir[4];
    TH1D *Channel[4];
    TH1D *Time[4];
    TH1D *Saturated[4];
    TH2D *ChannelTime[4];
    TH2D *ChannelSaturated[4];

    TH1D *Scaler;
    int last_scalercalc = 0;
    int last_scalersent = 0;
    TH1D* DiffScalerCalcTime;
    TH1D* DiffScalerSentTime;
};