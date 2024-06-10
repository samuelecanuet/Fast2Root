#include "QDC.hh"

QDC::QDC(string Name, int Label, int Coder, TFile *file, double TOTAL_TIME) : Detector(Name, Label, Coder, file, TOTAL_TIME)
{
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
        Coderdir[i] = DetectorDir->mkdir(("QDC" + to_string(i+1)).c_str());
        Channel[i] = new TH1D((Name + "_QDC" + to_string(i+1)+"_Channel").c_str(), (Name + "_QDC" + to_string(i+1)+"_Channel").c_str(), BIN, MIN, MAX);
        Time[i] = new TH1D((Name + "_QDC" + to_string(i+1)+"_Time").c_str(), (Name + "_QDC" + to_string(i+1)+"_Time").c_str(), TOTAL_TIME, 0, TOTAL_TIME);
        Saturated[i] = new TH1D((Name + "_QDC" + to_string(i+1)+"_Saturated").c_str(), (Name + "_QDC" + to_string(i+1)+"_Saturated").c_str(), BIN, MIN, MAX);

        ChannelTime[i] = new TH2D((Name + "_QDC" + to_string(i+1) + "_ChannelTime").c_str(), (Name + "_QDC" + to_string(i+1) + "_ChannelTime").c_str(), TOTAL_TIME, 0, TOTAL_TIME, BIN, MIN, MAX);
        ChannelSaturated[i] = new TH2D((Name + "_QDC" + to_string(i+1) + "_ChannelSaturated").c_str(), (Name + "_QDC" + to_string(i+1) + "_ChannelSaturated").c_str(), TOTAL_TIME, 0, TOTAL_TIME, BIN, MIN, MAX);
    }
}

void QDC::Fill(qdc_x1 value, int time)
{
    Channel[0]->Fill(value.q1);
    Time[0]->Fill(time);
    Saturated[0]->Fill(value.q1_saturated);
    ChannelTime[0]->Fill(time, value.q1);
    ChannelSaturated[0]->Fill(time, value.q1_saturated);
}

void QDC::Fill(qdc_x2 value, int time)
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
}

void QDC::Fill(qdc_x3 value, int time)
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

void QDC::Fill(qdc_x4 value, int time)
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
}
