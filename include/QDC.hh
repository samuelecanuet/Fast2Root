#include "Detector.hh"
using namespace std;

class QDC : public Detector
{
    public: 
    QDC(string Name, int Label, int Coder, TFile* file, double TOTAL_TIME);

    void Fill(qdc_x1 value, int Time) override;
    void Fill(qdc_x2 value, int Time) override;
    void Fill(qdc_x3 value, int Time) override;
    void Fill(qdc_x4 value, int Time) override;
    void Write() override ;

    int QDC_NB;
    TDirectory *Coderdir[4];
    TH1D *Channel[4];
    TH1D *Time[4];
    TH2I *ChannelTime[4];
};