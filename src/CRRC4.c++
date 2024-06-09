#include "CRRC4.hh"

CRRC4::CRRC4(string Name, int Label, int Coder, TFile *file, double TOTAL_TIME) : Detector(Name, Label, Coder, file, TOTAL_TIME)
{
    Coderdir = DetectorDir->mkdir("CRRC4");
    Channel = new TH1D((Name + "_CCRC4").c_str(), (Name + "_CCRC4").c_str(), BIN, MIN, MAX);
    ChannelTime = new TH2I((Name + "_CCRC4_Time").c_str(), (Name + "_CCRC4_Time").c_str(), TOTAL_TIME, 0, TOTAL_TIME, BIN, MIN, MAX);
}

void CRRC4::Fill(crrc4_spectro value, int Time)
{
    Channel->Fill(value.measure);
    ChannelTime->Fill(Time, value.measure);
}

void CRRC4::Write()
{
    DetectorDir->cd();
    Coderdir->cd();
    Channel->Write();
    ChannelTime->Write();
}