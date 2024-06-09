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
        Channel[i] = new TH1D((Name + "_QDC" + to_string(i+1)).c_str(), (Name + "_QDC" + to_string(i+1)).c_str(), BIN, MIN, MAX);
        ChannelTime[i] = new TH2I((Name + "_QDC" + to_string(i+1) + "_Time").c_str(), (Name + "_QDC" + to_string(i+1) + "_Time").c_str(), TOTAL_TIME, 0, TOTAL_TIME, BIN, MIN, MAX);
    }
}

void QDC::Fill(qdc_x1 value, int Time)
{
    Channel[0]->Fill(value.q1);
    ChannelTime[0]->Fill(Time, value.q1);
}

void QDC::Fill(qdc_x2 value, int Time)
{
    Channel[0]->Fill(value.q1);
    Channel[1]->Fill(value.q2);
    ChannelTime[0]->Fill(Time, value.q1);
    ChannelTime[1]->Fill(Time, value.q2);
}

void QDC::Fill(qdc_x3 value, int Time)
{
    Channel[0]->Fill(value.q1);
    Channel[1]->Fill(value.q2);
    Channel[2]->Fill(value.q3);
    ChannelTime[0]->Fill(Time, value.q1);
    ChannelTime[1]->Fill(Time, value.q2);
    ChannelTime[2]->Fill(Time, value.q3);
}

void QDC::Fill(qdc_x4 value, int Time)
{
    Channel[0]->Fill(value.q1);
    Channel[1]->Fill(value.q2);
    Channel[2]->Fill(value.q3);
    Channel[3]->Fill(value.q4);
    ChannelTime[0]->Fill(Time, value.q1);
    ChannelTime[1]->Fill(Time, value.q2);
    ChannelTime[2]->Fill(Time, value.q3);
    ChannelTime[3]->Fill(Time, value.q4);
}

void QDC::Write()
{
    DetectorDir->cd();
    for (int i = 0; i < QDC_NB; i++)
    {
        Coderdir[i]->cd();
        Channel[i]->Write();
        ChannelTime[i]->Write();
    }
}
