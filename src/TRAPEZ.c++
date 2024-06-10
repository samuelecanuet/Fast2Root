#include "TRAPEZ.hh"
#include "Detector.hh"

TRAPEZ::TRAPEZ(string Name, int Label, int Coder, TFile *file, double TOTAL_TIME) : Detector(Name, Label, Coder, file, TOTAL_TIME)
{
    Channel = new TH1D((Name + "_Channel").c_str(), (Name + "_Channel").c_str(), BIN, MIN, MAX);
    Time = new TH1D((Name + "_Time").c_str(), (Name + "_Time").c_str(), TOTAL_TIME, 0, TOTAL_TIME);
    PileUp = new TH1D((Name + "_PileUp").c_str(), (Name + "_PileUp").c_str(), BIN, MIN, MAX);
    Saturated = new TH1D((Name + "_Saturated").c_str(), (Name + "_Saturated").c_str(), BIN, MIN, MAX);

    ChannelTime = new TH2D((Name + "_ChannelTime").c_str(), (Name + "_ChannelTime").c_str(), TOTAL_TIME, 0, TOTAL_TIME, BIN, MIN, MAX);
    ChannelPileUp = new TH2D((Name + "_ChannelPileUp").c_str(), (Name + "_ChannelPileUp").c_str(), TOTAL_TIME, 0, TOTAL_TIME, BIN, MIN, MAX);
    ChannelSaturated = new TH2D((Name + "_ChannelSaturated").c_str(), (Name + "_ChannelSaturated").c_str(), TOTAL_TIME, 0, TOTAL_TIME, BIN, MIN, MAX);
}

void TRAPEZ::Fill(trapez_spectro value, int time)
{
    Channel->Fill(value.measure);
    Time->Fill(time);
    PileUp->Fill(value.pileup);
    Saturated->Fill(value.saturated);

    ChannelTime->Fill(time, value.measure);
    ChannelPileUp->Fill(time, value.pileup);
    ChannelSaturated->Fill(time, value.saturated);
}

void TRAPEZ::Write()
{
    DetectorDir->cd();
    Channel->Write();
    Time->Write();
    PileUp->Write();
    Saturated->Write();

    ChannelTime->Write();
    ChannelPileUp->Write();
    ChannelSaturated->Write();
}
