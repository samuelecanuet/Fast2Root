#include "TRAPEZ.hh"
#include "Detector.hh"

TRAPEZ::TRAPEZ(string Name, int Label, int Coder, TFile *file, double TOTAL_TIME, string setupFile) : Detector(Name, Label, Coder, file, TOTAL_TIME, setupFile)
{    
    SetRangesFromFile();
    Channel = new TH1D((Name + "_Channel").c_str(), (Name + "_Channel").c_str(), BIN, MIN, MAX);
    Time = new TH1D((Name + "_Time").c_str(), (Name + "_Time").c_str(), TOTAL_TIME, 0, TOTAL_TIME);
    PileUp = new TH1D((Name + "_PileUp").c_str(), (Name + "_PileUp").c_str(), 2, 0,2);
    Saturated = new TH1D((Name + "_Saturated").c_str(), (Name + "_Saturated").c_str(), 2, 0, 2);

    ChannelTime = new TH2D((Name + "_ChannelTime").c_str(), (Name + "_ChannelTime").c_str(), TOTAL_TIME, 0, TOTAL_TIME, BIN, MIN, MAX);
    ChannelPileUp = new TH1D((Name + "_ChannelPileUp").c_str(), (Name + "_ChannelPileUp").c_str(), BIN, MIN, MAX);
    ChannelSaturated = new TH1D((Name + "_ChannelSaturated").c_str(), (Name + "_ChannelSaturated").c_str(), BIN, MIN, MAX);

    WriteSetupPropreties(Name, setupFile);
}

void TRAPEZ::Fill(trapez_spectro value, double time)
{
    Channel->Fill(value.measure);
    Time->Fill(time);
    PileUp->Fill(value.pileup);
    Saturated->Fill(value.saturated);

    ChannelTime->Fill(time, value.measure);
    if (value.pileup)
        ChannelPileUp->Fill(value.measure);
    if (value.saturated)
        ChannelSaturated->Fill(value.measure);
}

void TRAPEZ::Fill(spectro_counter value, double time)
{
    DiffScalerTrigTime->Fill(time, value.trig - last_scalertrig);
    DiffScalerSentTime->Fill(time, value.sent - last_scalersent);
    DiffScalerCalcTime->Fill(time, value.calc - last_scalercalc);

    last_scalertrig = value.trig;
    last_scalersent = value.sent;
    last_scalercalc = value.calc;
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

    DiffScalerTrigTime->Write();
    DiffScalerSentTime->Write();
    DiffScalerCalcTime->Write();
    Scaler->SetBinContent(1, DiffScalerTrigTime->Integral());
    Scaler->SetBinContent(2, DiffScalerSentTime->Integral());
    Scaler->SetBinContent(3, DiffScalerCalcTime->Integral());
    Scaler->Write();
}
