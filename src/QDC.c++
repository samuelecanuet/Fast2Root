#include "QDC.hh"

QDC::QDC(string Name, int Label, int Coder, TFile *file, double TOTAL_TIME, string setupFile) : Detector(Name, Label, Coder, file, TOTAL_TIME, setupFile)
{
    SetRangesFromFile();
    WriteSetupPropreties(Name, setupFile);

    int bin_time = 10000; 

    if (Coder == QDC_X1_TYPE_ALIAS)
        QDC_NB = 1;
    else if (Coder == QDC_X2_TYPE_ALIAS)
        QDC_NB = 2;
    else if (Coder == QDC_X3_TYPE_ALIAS)
        QDC_NB = 3;
    else if (Coder == QDC_X4_TYPE_ALIAS)
        QDC_NB = 4;
    else
        QDC_NB = 4;

    for (int i = 0; i < QDC_NB; i++)
    {
        Coderdir[i] = DetectorDir->mkdir(("QDC" + to_string(i + 1)).c_str());
        Channel[i] = new TH1D((Name + "_QDC" + to_string(i + 1) + "_Channel").c_str(), (Name + "_QDC" + to_string(i + 1) + "_Channel").c_str(), BIN, MIN, MAX);
        Time[i] = new TH1D((Name + "_QDC" + to_string(i + 1) + "_Time").c_str(), (Name + "_QDC" + to_string(i + 1) + "_Time").c_str(), bin_time, 0, TOTAL_TIME);
        Saturated[i] = new TH1D((Name + "_QDC" + to_string(i + 1) + "_Saturated").c_str(), (Name + "_QDC" + to_string(i + 1) + "_Saturated").c_str(), BIN, MIN, MAX);

        ChannelTime[i] = new TH2D((Name + "_QDC" + to_string(i + 1) + "_ChannelTime").c_str(), (Name + "_QDC" + to_string(i + 1) + "_ChannelTime").c_str(), bin_time / 10, 0, TOTAL_TIME, BIN, MIN, MAX);
        ChannelSaturated[i] = new TH2D((Name + "_QDC" + to_string(i + 1) + "_ChannelSaturated").c_str(), (Name + "_QDC" + to_string(i + 1) + "_ChannelSaturated").c_str(), bin_time / 10, 0, TOTAL_TIME, BIN, MIN, MAX);
    }

    if (QDC_NB > 1)
    {
        ChannelQDC1QDC2 = new TH2D((Name + "_ChannelQDC1QDC2").c_str(), (Name + "_ChannelQDC1QDC2").c_str(), BIN/10, MIN, MAX, BIN/10, -MAX, MAX);
        ChannelQDC1QDC2->GetXaxis()->SetTitle(("QDC1 (" + to_string(int(QDC1Range.first)) + "ns : " + to_string(int(QDC1Range.second)) + "ns) [Channel]").c_str());
        ChannelQDC1QDC2->GetYaxis()->SetTitle(("QDC2 (" + to_string(int(QDC2Range.first)) + "ns : " + to_string(int(QDC2Range.second)) + "ns) [Channel]").c_str());
    }

    DiffScalerSentTime = new TH1D((Name  + "_DiffScalerSentTime").c_str(), (Name  + "_DiffScalerSentTime").c_str(), TOTAL_TIME, 0, TOTAL_TIME);
    DiffScalerCalcTime = new TH1D((Name  + "_DiffScalerCalcTime").c_str(), (Name  + "_DiffScalerCalcTime").c_str(), TOTAL_TIME, 0, TOTAL_TIME);
    Scaler = new TH1D((Name  + "_Scaler").c_str(), (Name  + "_Scaler").c_str(), 2, 0, 2);

    
}

void QDC::Fill(qdc_x1 value, double time)
{
    Channel[0]->Fill(value.q1);
    Time[0]->Fill(time);
    Saturated[0]->Fill(value.q1_saturated);
    ChannelTime[0]->Fill(time, value.q1);
    ChannelSaturated[0]->Fill(time, value.q1_saturated);
}

void QDC::Fill(qdc_x2 value, double time)
{
    Channel[0]->Fill(value.q1);
    Channel[1]->Fill(value.q2);
    Time[0]->Fill(time);
    Time[1]->Fill(time);
    Saturated[0]->Fill(value.q1_saturated);
    Saturated[1]->Fill(value.q2_saturated);
    ChannelTime[0]->Fill(time, value.q1);
    ChannelTime[1]->Fill(time, value.q2);
    ChannelSaturated[0]->Fill(time, value.q1_saturated);
    ChannelSaturated[1]->Fill(time, value.q2_saturated);

    if (value.q1 != 0)
        ChannelQDC1QDC2->Fill(value.q1, (double)value.q2);
}

void QDC::Fill(qdc_x3 value, double time)
{
    Channel[0]->Fill(value.q1);
    Channel[1]->Fill(value.q2);
    Channel[2]->Fill(value.q3);
    Time[0]->Fill(time);
    Time[1]->Fill(time);
    Time[2]->Fill(time);
    Saturated[0]->Fill(value.q1_saturated);
    Saturated[1]->Fill(value.q2_saturated);
    Saturated[2]->Fill(value.q3_saturated);
    ChannelTime[0]->Fill(time, value.q1);
    ChannelTime[1]->Fill(time, value.q2);
    ChannelTime[2]->Fill(time, value.q3);
    ChannelSaturated[0]->Fill(time, value.q1_saturated);
    ChannelSaturated[1]->Fill(time, value.q2_saturated);
    ChannelSaturated[2]->Fill(time, value.q3_saturated);
}

void QDC::Fill(qdc_x4 value, double time)
{
    Channel[0]->Fill(value.q1);
    Channel[1]->Fill(value.q2);
    Channel[2]->Fill(value.q3);
    Channel[3]->Fill(value.q4);
    Time[0]->Fill(time);
    Time[1]->Fill(time);
    Time[2]->Fill(time);
    Time[3]->Fill(time);
    Saturated[0]->Fill(value.q1_saturated);
    Saturated[1]->Fill(value.q2_saturated);
    Saturated[2]->Fill(value.q3_saturated);
    Saturated[3]->Fill(value.q4_saturated);
    ChannelTime[0]->Fill(time, value.q1);
    ChannelTime[1]->Fill(time, value.q2);
    ChannelTime[2]->Fill(time, value.q3);
    ChannelTime[3]->Fill(time, value.q4);
    ChannelSaturated[0]->Fill(time, value.q1_saturated);
    ChannelSaturated[1]->Fill(time, value.q2_saturated);
    ChannelSaturated[2]->Fill(time, value.q3_saturated);
    ChannelSaturated[3]->Fill(time, value.q4_saturated);
}

void QDC::Fill(qdc_counter value, double time)
{
    DiffScalerCalcTime->Fill(time, value.calc - last_scalercalc);
    DiffScalerSentTime->Fill(time, value.sent - last_scalersent);
    last_scalercalc = value.calc;
    last_scalersent = value.sent;
}

void QDC::Write()
{
    
    DetectorDir->cd();
    for (int i = 0; i < QDC_NB; i++)
    {
        Coderdir[i]->cd();
        Channel[i]->Write();
        Time[i]->Write();
        Saturated[i]->Write();
        ChannelTime[i]->Write();
        ChannelSaturated[i]->Write();
    }

    DetectorDir->cd();
    ChannelQDC1QDC2->Write();
    DiffScalerCalcTime->Write();
    DiffScalerSentTime->Write();
    Scaler->SetBinContent(1, DiffScalerCalcTime->Integral());
    Scaler->SetBinContent(2, DiffScalerSentTime->Integral());
    Scaler->Write();
}
