#include "Detector.hh"
#include "Messenger.hh"

Detector::Detector(string Name, int Label, int Coder, TFile *file, double TOTAL_TIME, string setupFile)
{
    NAME = Name;
    LABEL = Label;
    CODER = Coder;

    SetRangesFromFile();

    DetectorDir = file->mkdir(NAME.c_str());
    DetectorPropretiesDir = DetectorDir->mkdir("Propreties");

    if (CODER == QDC_X1_TYPE_ALIAS || CODER == QDC_X2_TYPE_ALIAS || CODER == QDC_X3_TYPE_ALIAS || CODER == QDC_X4_TYPE_ALIAS || CODER == CRRC4_SPECTRO_TYPE_ALIAS)
    {
    }
    else
    {
        F2RWarning(("Coder " + string(type_name(Coder)) + " not implemented yet. " + NAME + " will not be saved.").c_str());
    }

}

Detector::~Detector()
{
}

void Detector::Fill(qdc_x1 value, double Time)
{
}

void Detector::Fill(qdc_x2 value, double Time)
{
}

void Detector::Fill(qdc_x3 value, double Time)
{
}

void Detector::Fill(qdc_x4 value, double Time)
{
}

void Detector::Fill(crrc4_spectro value, double Time)
{
}

void Detector::Fill(trapez_spectro value, double Time)
{
}

void Detector::Fill(spectro_counter value, double Time)
{
}

void Detector::Fill(qdc_counter value, double Time)
{
}

void Detector::Write()
{
}

int Detector::SetRangesFromFile()
{
    string filename = "Range_Input.txt";
    ifstream inputfile(filename.c_str());

    string line;
    while (getline(inputfile, line))
    {
        if (line.empty())
            continue;

        istringstream iss(line);
        string label, bins, min, max;
        string name;
        iss >> label >> name;
        if (stoi(label) == LABEL)
        {
            if (!(iss >> bins >> min >> max))
            {
                BIN = 1000;
                MIN = 0;
                MAX = 100000;
                DEFAULT_RANGES = true;
                inputfile.close();
                return 0;
            }
            else if (stoi(label) == LABEL)
            {
                BIN = stoi(bins);
                MIN = stoi(min);
                MAX = stoi(max);
                inputfile.close();
                return 0;
            }
        }
    }
    inputfile.close();
    F2RError(NAME + " not in Range_Input.txt file !");
    exit(0);
}

string Detector::GetName()
{
    return NAME;
}

int Detector::GetLabel()
{
    return LABEL;
}

int Detector::GetCoder()
{
    return CODER;
}

bool Detector::GetDefaultRanges()
{
    return DEFAULT_RANGES;
}

void Detector::WriteSetupPropreties(string name, string filename)
{
    DetectorPropretiesDir->cd();
    ifstream file(filename);
    string line;

    bool DetectorFound = false;
    int line_counter = 0;
    int start_line = 0;
    
    if (file.is_open())
    {
        while (getline(file, line))
        {
            line_counter += 1;
            if (line.find("Channel_Enabled") != string::npos)
            {
                start_line = line_counter;
            }
            else if (line.find("DSP_Label") != string::npos)
            {
                istringstream iss(line);
                string token, equal, value;
                iss >> token >> equal >> value;

                if (value == to_string(LABEL))
                {
                    break;
                }
            }
        }

        // return to first line
        file.clear();
        file.seekg(0, ios::beg);

        line_counter = 0;

        while (getline(file, line))
        {
            if (line_counter == start_line)
            {
                DetectorFound = true;
            }

            if (DetectorFound)
            {
                DetectorPropretiesDir->cd();
                istringstream iss(line);
                string token, equal, value;
                iss >> token >> equal >> value;

                // avoid dummy lines
                if (equal != "=")
                    continue;
                
                TNamed *prop = new TNamed(token.c_str(), value.c_str());
                prop->Write();

                if (token == "DSP_Q1_Begin")
                    QDC1Range = make_pair(stod(value), 0);
                if (token == "DSP_Q1_End")
                    QDC1Range.second = stod(value);
                if (token == "DSP_Q2_Begin")
                    QDC2Range = make_pair(stod(value), 0);
                if (token == "DSP_Q2_End")
                    QDC2Range.second = stod(value);
                if (token == "DSP_Q3_Begin")
                    QDC3Range = make_pair(stod(value), 0);
                if (token == "DSP_Q3_End")
                    QDC3Range.second = stod(value);
                if (token == "DSP_Q4_Begin")
                    QDC4Range = make_pair(stod(value), 0);

                // end of taking data
                if (token == "State_Enabled")
                    break;
                
            }

            line_counter += 1;
        }
    }
    else
    {
        F2RError("Impossible to open Setup file");
        exit(0);
    }
}