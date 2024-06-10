#ifndef UTILITIES_HH
#define UTILITIES_HH

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <variant>
#include <map>
#include <ctime>
#include <iomanip>
#include <filesystem>

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"
#include "TH2I.h"
#include "TCanvas.h"

#include "fasterac/fasterac.h"
#include "fasterac/fast_data.h"
#include "fasterac/qdc.h"
#include "fasterac/spectro.h"
#include "fasterac/group.h"

#include "Detector.hh"
#include "QDC.hh"
#include "CRRC4.hh"
#include "TRAPEZ.hh"

#ifdef USE_SIGNAL_DICT
#include "../../lib/SignalDict/Signal.h"
#endif

using namespace std;

struct ChannelInfo
{
    string name;
    int label;
    int coder;
    int dspNbQ = 0;
};

string setupFile;
string inputrange;
vector<Detector *> Detectors;
TFile *ROOTFile;
string filename_base;
double TOTAL_TIME;

TTree *Tree;
TTree *Tree_Group;

vector<double> Channel_vec;
vector<double> Time_vec;
vector<int> Label_vec;
double Channel;
double Time;
int Label;

#ifdef USE_SIGNAL_DICT
vector<Signal> signal_vec;
Signal signal;
#endif

bool HISTOGRAMS_RANGES = false;
/************/
/*  FASTER  */
/************/
//  file reader
faster_file_reader_p reader;
int file_size = 0;
int counter = 0;
//  data
faster_data_p data;
faster_data_p group_data;
unsigned char coder;
unsigned short label;
double clock_ns;
double clock_sec;
int channel;
int counter_time = 0;
double START_TIME;

int sum = 0;

faster_buffer_reader_p group_reader;
char group_buffer[1500];
unsigned short lsize;

qdc_x1 qdc1;
qdc_x2 qdc2;
qdc_x3 qdc3;
qdc_x4 qdc4;
crrc4_spectro spectro_data;
trapez_spectro trapez_data;

bool Coder_Not_Implemented[999];

clock_t start = clock(), Current;

map<string, ChannelInfo> InitDetectors(const string &filePath)
{
    map<string, ChannelInfo> channelInfoMap;

    ifstream file(filePath);
    if (!file.is_open())
    {
        F2RError(("Impossible to open Setup file : " + filePath).c_str());
        return channelInfoMap;
    }

    string line;
    string currentChannelName;
    string equal;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        istringstream iss(line);        cout << detector->GetName() << endl;

        string token;
        iss >> token;
        if (token == ":")
        {
            iss >> currentChannelName;
            ChannelInfo channelInfo;
            channelInfo.name = currentChannelName;
            channelInfoMap[currentChannelName] = channelInfo;
        }
        else if (token == "DSP_Label")
        {
            ChannelInfo &channelInfo = channelInfoMap[currentChannelName];
            iss >> equal >> channelInfo.label;
        }
        else if (token == "DSP_Type_Alias")
        {
            ChannelInfo &channelInfo = channelInfoMap[currentChannelName];
            iss >> equal >> channelInfo.coder;
        }
        else if (token == "DSP_Nb_Q")
        {
            ChannelInfo &channelInfo = channelInfoMap[currentChannelName];
            iss >> equal >> channelInfo.dspNbQ;
        }
    }

    file.close();

    if (!HISTOGRAMS_RANGES)
    {
        ofstream outFile("Range_Input.txt");
        F2RInfo("Creating default Range_Input.txt file");

        for (const auto &pair : channelInfoMap)
        {
            outFile << pair.second.label << "\t" << pair.second.name << endl;
        }

        outFile.close();
    }

    string output;
    for (const auto &pair : channelInfoMap)
    {
        if (Detectors.size() < pair.second.label + 1)
        {
            Detectors.resize(pair.second.label + 1);
        }

        if (pair.second.coder == QDC_X1_TYPE_ALIAS || pair.second.coder == QDC_X2_TYPE_ALIAS || pair.second.coder == QDC_X3_TYPE_ALIAS || pair.second.coder == QDC_X4_TYPE_ALIAS)
            Detectors[pair.second.label] = new QDC(pair.second.name, pair.second.label, pair.second.coder, ROOTFile, TOTAL_TIME);
        else if (pair.second.coder == CRRC4_SPECTRO_TYPE_ALIAS)
            Detectors[pair.second.label] = new CRRC4(pair.second.name, pair.second.label, pair.second.coder, ROOTFile, TOTAL_TIME);
        else if (pair.second.coder == TRAPEZ_SPECTRO_TYPE_ALIAS)
            Detectors[pair.second.label] = new TRAPEZ(pair.second.name, pair.second.label, pair.second.coder, ROOTFile, TOTAL_TIME);
        else
            Detectors[pair.second.label] = new Detector(pair.second.name, pair.second.label, pair.second.coder, ROOTFile, TOTAL_TIME);
        
        if (Detectors[pair.second.label]->GetDefaultRanges())
        {
            output +=  Detectors[pair.second.label]->GetName();
            output += " \t ";
        }
    }
    
    if (!output.empty())
    F2RInfo("Default ranges for : " + output);

    return channelInfoMap;
}

void WriteHistograms()
{
    ROOTFile->cd();
    for (auto detector : Detectors)
    {
        if (detector == nullptr)
            continue;
        detector->Write();
    }
    ROOTFile->cd();
}

int ReadRunTime()
{
    int error = 0;

    string start_time;
    string stop_time;
    string line;

    ifstream file(setupFile);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line.find("- Start date") != string::npos)
            {
                start_time = line.substr(17, 50);
            }
            if (line.find("- Stop date") != string::npos)
            {
                stop_time = line.substr(16, 50);
                break;
            }
        }
        file.close();

        tm tm;
        istringstream ss(start_time);
        ss >> get_time(&tm, "%d-%m-%Y %H:%M:%S");
        time_t start_seconds = mktime(&tm);

        ss.clear();
        ss.str(stop_time);
        ss >> get_time(&tm, "%d-%m-%Y %H:%M:%S");
        time_t stop_seconds = mktime(&tm);

        TOTAL_TIME = difftime(stop_seconds, start_seconds);
    }

    return (error);
}

int GetChannel(faster_data_p data) 
{
    if (coder == QDC_X1_TYPE_ALIAS)
    {
        faster_data_load(data, &qdc1);
        Channel_vec.push_back(qdc1.q1);
        Detectors[label]->Fill(qdc1, clock_ns);
        return qdc1.q1;
    }
    else if (coder == QDC_X2_TYPE_ALIAS)
    {
        faster_data_load(data, &qdc2);
        Channel_vec.push_back(qdc2.q1);
        Detectors[label]->Fill(qdc2, clock_ns);
        return qdc2.q1;
    }
    else if (coder == QDC_X3_TYPE_ALIAS)
    {
        faster_data_load(data, &qdc3);
        Channel_vec.push_back(qdc3.q1);
        Detectors[label]->Fill(qdc3, clock_ns);
        return qdc3.q1;
    }
    else if (coder == QDC_X4_TYPE_ALIAS)
    {
        faster_data_load(data, &qdc4);
        Channel_vec.push_back(qdc4.q1);
        Detectors[label]->Fill(qdc4, clock_ns);
        return qdc4.q1;
    }
    else if (coder == CRRC4_SPECTRO_TYPE_ALIAS)
    {
        faster_data_load(data, &spectro_data);
        Channel_vec.push_back(spectro_data.measure);
        Detectors[label]->Fill(spectro_data, clock_ns);
        return spectro_data.measure;
    }
    else if (coder == TRAPEZ_SPECTRO_TYPE_ALIAS)
    {
        faster_data_load(data, &trapez_data);
        Channel_vec.push_back(trapez_data.measure);
        Detectors[label]->Fill(trapez_data, clock_ns);
        return trapez_data.measure;
    }
    else
    {
        if (!Coder_Not_Implemented[coder])
        {
            F2RWarning(Form(("Coder " + string(type_name(coder)) + " not implemented yet").c_str()));
            Coder_Not_Implemented[coder] = true;
        }
        return -1;
    }
}

int Filling()
{

    if (coder == GROUP_TYPE_ALIAS)
    {
        lsize = faster_data_load(data, group_buffer);
        group_reader = faster_buffer_reader_open(group_buffer, lsize);
        while ((data = faster_buffer_reader_next(group_reader)) != NULL)
        {
            coder = faster_data_type_alias(data);
            label = faster_data_label(data);
            clock_ns = faster_data_clock_ns(data);
            channel = GetChannel(data);

            if (channel == -1)
                continue;
            
            #ifdef USE_SIGNAL_DICT
            signal_vec.push_back(Signal(label, clock_ns, channel));
            #endif

            #ifndef USE_SIGNAL_DICT
            Label_vec.push_back(label);
            Time_vec.push_back(clock_ns);
            Channel_vec.push_back(channel);
            #endif
            
        }

        faster_buffer_reader_close(group_reader);   
        Tree_Group->Fill();

        #ifdef USE_SIGNAL_DICT
        signal_vec.clear();
        #endif
        #ifndef USE_SIGNAL_DICT
        Label_vec.clear();
        Time_vec.clear();
        Channel_vec.clear();
        #endif

        return 0;
    }
    else
    {
        Channel = GetChannel(data);

        if (Channel == -1)
            return 0;
        #ifdef USE_SIGNAL_DICT
        signal = Signal(label, clock_ns, Channel);
        #endif
        #ifndef USE_SIGNAL_DICT
        Label = label;
        Time = clock_ns;
        #endif
        Tree->Fill();
        return 0;
    }
}

#endif