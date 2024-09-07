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
#include <sys/stat.h>

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
};

string setupFile;
string inputrange;
string fast_filename_raw;
vector<Detector *> Detectors;
TFile *ROOTFile;
string filename_base;
double TOTAL_TIME;

TTree *Tree;
TTree *Tree_Group;

vector<double> Channel_vec;
vector<double> Time_vec;
vector<int> Label_vec;
vector<int> PileUp_vec;
double Channel;
int PileUp;
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
faster_data_p _data;
faster_data_p group_data;
unsigned char coder;
unsigned short label;
double clock_ns;
double clock_sec;
pair<int, int> channelpileup;
int channel;
int pileup;
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

    //////////////////////READING SETUP FILE//////////////////////
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

        istringstream iss(line);

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
            if (currentChannelName.empty())
            {
                F2RInfo("File was genereted by multifast. Reading sample.pid file.");
                break;
            }
            ChannelInfo &channelInfo = channelInfoMap[currentChannelName];
            iss >> equal >> channelInfo.label;
        }
        else if (token == "DSP_Type_Alias")
        {
            if (currentChannelName.empty())
            {
                F2RInfo("File was genereted by multifast. Reading sample.pid file.");
                break;
            }
            ChannelInfo &channelInfo = channelInfoMap[currentChannelName];
            iss >> equal >> channelInfo.coder;
        }
    }
    file.close();

    //////////////////////READING SAMPLE.PID FILE FOR MULTIFAST MODE//////////////////////
    if (channelInfoMap.empty())
    {
        ifstream file("sample.pid");
        if (!file.is_open())
        {
            F2RError("Impossible to open sample.pid file");
            exit(0);
            return channelInfoMap;
        }

        string line;
        string currentChannelName;
        string equal;
        while (getline(file, line))
        {

            if (line.find("#") != string::npos)
                continue;

            istringstream iss(line);

            size_t lastDelimiterPos = line.rfind(':');
            size_t firstDelimiterPos = line.find(':');
            string currentChannelName = line.substr(lastDelimiterPos + 1);
            string currentLabel = line.substr(0, firstDelimiterPos);
            ChannelInfo channelInfo;
            channelInfo.name = currentChannelName;
            channelInfoMap[currentChannelName] = channelInfo;
            channelInfoMap[currentChannelName].label = stoi(currentLabel);
        }
        file.close();

        //////////////////////READING SETUP FILE FOR MULTIFAST MODE//////////////////////
        ifstream ofile(filePath);
        if (!ofile.is_open())
        {
            F2RError(("Impossible to open Setup file : " + filePath).c_str());
            exit(0);
            return channelInfoMap;
        }

        int try_label;
        while (getline(ofile, line))
        {
            if (line.empty())
                continue;

            istringstream iss(line);

            string token;
            iss >> token;
            if (token == "DSP_Label")
            {
                iss >> equal >> try_label;
                for (const auto &pair : channelInfoMap)
                {
                    if (pair.second.label == try_label)
                    {
                        currentChannelName = pair.second.name;
                        break;
                    }
                }
            }
            else if (token == "DSP_Type_Alias")
            {
                ChannelInfo &channelInfo = channelInfoMap[currentChannelName];
                iss >> equal >> channelInfo.coder;
            }
        }
        file.close();
    }

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
        {
            Detectors[pair.second.label] = new QDC(pair.second.name, pair.second.label, pair.second.coder, ROOTFile, TOTAL_TIME);
        }
        else if (pair.second.coder == CRRC4_SPECTRO_TYPE_ALIAS)
        {
            Detectors[pair.second.label] = new CRRC4(pair.second.name, pair.second.label, pair.second.coder, ROOTFile, TOTAL_TIME);
        }
        else if (pair.second.coder == TRAPEZ_SPECTRO_TYPE_ALIAS)
        {
            Detectors[pair.second.label] = new TRAPEZ(pair.second.name, pair.second.label, pair.second.coder, ROOTFile, TOTAL_TIME);
        }
        else
        {
            Detectors[pair.second.label] = new Detector(pair.second.name, pair.second.label, pair.second.coder, ROOTFile, TOTAL_TIME);
        }

        if (Detectors[pair.second.label]->GetDefaultRanges())
        {
            output += Detectors[pair.second.label]->GetName();
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

pair<string, string> ReadRunTime()
{
    int error = 0;

    string start_time;
    string stop_time = " ";
    string line;
    bool multifast = true;

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
                multifast = false;
                stop_time = line.substr(16, 50);
                break;
            }
        }
    }
    else
    {
        F2RError("Impossible to open Setup file");
        exit(0);
    }
    file.close();

    if (multifast)
    {
        ifstream ofile(setupFile);
        ofile.is_open();
        while (getline(ofile, line))
        {
            if (line.find("- Start date") != string::npos)
            {
                start_time = line.substr(17, 50);
            }
            if (line.find("- Stop  date") != string::npos)
            {
                stop_time = line.substr(17, 50);
                break;
            }
        }
        ofile.close();
    }

    time_t stop_seconds;
    time_t start_seconds;

    if (multifast)
    {
        tm tm;
        istringstream ss(start_time);
        ss >> get_time(&tm, "%Y-%m-%d at %Hh-%Mm-%Ss");
        start_seconds = mktime(&tm);

        ss.clear();
        ss.str(stop_time);
        ss >> get_time(&tm, "%Y-%m-%d at %Hh-%Mm-%Ss");
        stop_seconds = mktime(&tm);
    }
    else
    {
        tm tm;
        istringstream ss(start_time);
        ss >> get_time(&tm, "%d-%m-%Y %H:%M:%S");
        start_seconds = mktime(&tm);

        ss.clear();
        ss.str(stop_time);
        ss >> get_time(&tm, "%d-%m-%Y %H:%M:%S");
        stop_seconds = mktime(&tm);
    }

    

    TOTAL_TIME = difftime(stop_seconds, start_seconds);

    return make_pair(start_time, stop_time);
}

pair<int, int> GetChannel(faster_data_p _data)
{
    if (coder == QDC_X1_TYPE_ALIAS)
    {
        faster_data_load(_data, &qdc1);
        Detectors[label]->Fill(qdc1, clock_ns);
        return make_pair((int)qdc1.q1, 0);
    }
    else if (coder == QDC_X2_TYPE_ALIAS)
    {
        faster_data_load(_data, &qdc2);
        Detectors[label]->Fill(qdc2, clock_ns);
        return make_pair((int)qdc2.q1, 0);
    }
    else if (coder == QDC_X3_TYPE_ALIAS)
    {
        faster_data_load(_data, &qdc3);
        Detectors[label]->Fill(qdc3, clock_ns);
        return make_pair((int)qdc3.q1, 0);
    }
    else if (coder == QDC_X4_TYPE_ALIAS)
    {
        faster_data_load(_data, &qdc4);
        Detectors[label]->Fill(qdc4, clock_ns);
        return make_pair((int)qdc4.q1, 0);
    }
    else if (coder == CRRC4_SPECTRO_TYPE_ALIAS)
    {
        faster_data_load(_data, &spectro_data);
        Detectors[label]->Fill(spectro_data, clock_ns);
        return make_pair((int)spectro_data.measure, (int)spectro_data.pileup);
    }
    else if (coder == TRAPEZ_SPECTRO_TYPE_ALIAS)
    {
        faster_data_load(_data, &trapez_data);
        Detectors[label]->Fill(trapez_data, clock_ns);
        return make_pair((int)trapez_data.measure, (int)trapez_data.pileup);
    }
    else
    {
        if (!Coder_Not_Implemented[coder])
        {
            F2RWarning(Form(("Coder " + string(type_name(coder)) + " not implemented yet").c_str()));
            Coder_Not_Implemented[coder] = true;
        }
        return make_pair(-1, -1);
    }
}

int Filling()
{

    if (coder == GROUP_TYPE_ALIAS)
    {
        lsize = faster_data_load(_data, group_buffer);
        group_reader = faster_buffer_reader_open(group_buffer, lsize);
        while ((_data = faster_buffer_reader_next(group_reader)) != NULL)
        {
            coder = faster_data_type_alias(_data);
            label = faster_data_label(_data);
            clock_ns = faster_data_clock_ns(_data);
            channelpileup = GetChannel(_data);
            channel = channelpileup.first;
            pileup = channelpileup.second;

            if (channel == -1)
                continue;

#ifdef USE_SIGNAL_DICT
            signal_vec.push_back(Signal(label, clock_ns, channel, pileup));
#endif

#ifndef USE_SIGNAL_DICT
            Label_vec.push_back(label);
            Time_vec.push_back(clock_ns);
            Channel_vec.push_back(channel);
            PileUp_vec.push_back(pileup);
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
        PileUp_vec.clear();
#endif

        return 0;
    }
    else
    {
        channelpileup = GetChannel(_data);
        Channel = channelpileup.first;
        PileUp = channelpileup.second;

        if (Channel == -1)
            return 0;
#ifdef USE_SIGNAL_DICT
        signal = Signal(label, clock_ns, Channel, pileup);
#endif
#ifndef USE_SIGNAL_DICT
        Label = label;
        Time = clock_ns;
#endif
        Tree->Fill();
        return 0;
    }
}

bool FolderExists(const string &folderPath)
{
    struct stat info;

    if (stat(folderPath.c_str(), &info) != 0)
    {
        return false;
    }
    else if (info.st_mode & S_IFDIR)
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif