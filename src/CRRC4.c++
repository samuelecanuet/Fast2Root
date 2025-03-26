#include "CRRC4.hh"

CRRC4::CRRC4(string Name, int Label, int Coder, TFile *file, double TOTAL_TIME, string setupFile) : 
    Detector(Name, Label, Coder, file, TOTAL_TIME, setupFile)
{
    SetRangesFromFile();
    int bin_time = 10000;  

    Channel = new TH1D((Name + "_Channel").c_str(), (Name + "_Channel").c_str(), BIN, MIN, MAX);
    Time = new TH1D((Name + "_Time").c_str(), (Name + "_Time").c_str(), bin_time, 0, TOTAL_TIME);
    PileUp = new TH1D((Name + "_PileUp").c_str(), (Name + "_PileUp").c_str(), 2, 0, 2);
    Saturated = new TH1D((Name + "_Saturated").c_str(), (Name + "_Saturated").c_str(), 2, 0, 2);

    ChannelTime = new TH2D((Name + "_ChannelTime").c_str(), (Name + "_ChannelTime").c_str(), bin_time/10, 0, TOTAL_TIME, BIN, MIN, MAX);
    ChannelPileUp = new TH1D((Name + "_ChannelPileUp").c_str(), (Name + "_ChannelPileUp").c_str(), BIN, MIN, MAX);
    ChannelSaturated = new TH1D((Name + "_ChannelSaturated").c_str(), (Name + "_ChannelSaturated").c_str(), BIN, MIN, MAX);

    DiffScalerTrigTime = new TH1D((Name + "_DiffScalerTrigTime").c_str(), (Name + "_DiffScalerTrigTime").c_str(), TOTAL_TIME, 0, TOTAL_TIME);
    DiffScalerSentTime = new TH1D((Name + "_DiffScalerSentTime").c_str(), (Name + "_DiffScalerSentTime").c_str(), TOTAL_TIME, 0, TOTAL_TIME);
    DiffScalerCalcTime = new TH1D((Name + "_DiffScalerCalcTime").c_str(), (Name + "_DiffScalerCalcTime").c_str(), TOTAL_TIME, 0, TOTAL_TIME);
    Scaler = new TH1D((Name + "_Scaler").c_str(), (Name + "_Scaler").c_str(), 3, 0, 3);

    WriteSetupPropreties(Name, setupFile);
}

void CRRC4::Fill(crrc4_spectro value, double time)
{
    Channel->Fill(value.measure);
    Time->Fill(time);
    PileUp->Fill(value.pileup);
    Saturated->Fill(value.saturated);

    ChannelTime->Fill(time, value.measure);
    ChannelTime->Fill(time, value.measure);
    if (value.pileup)
        ChannelPileUp->Fill(value.measure);
    if (value.saturated)
        ChannelSaturated->Fill(value.measure);
}

void CRRC4::Fill(spectro_counter value, double time)
{
    DiffScalerTrigTime->Fill(time, value.trig - last_scalertrig);
    DiffScalerSentTime->Fill(time, value.sent - last_scalersent);
    DiffScalerCalcTime->Fill(time, value.calc - last_scalercalc);

    last_scalercalc = value.calc;
    last_scalersent = value.sent;
    last_scalertrig = value.trig;
}

void CRRC4::Write()
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