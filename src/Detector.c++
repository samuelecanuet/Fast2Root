#include "Detector.hh"
#include "Messenger.hh"

Detector::Detector(string Name, int Label, int Coder, TFile *file, double TOTAL_TIME)
{
    NAME = Name;
    LABEL = Label;
    CODER = Coder;

    SetRangesFromFile();

    DetectorDir = file->mkdir(NAME.c_str());

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

void Detector::Fill(qdc_x1 value, int Time)
{
}

void Detector::Fill(qdc_x2 value, int Time)
{
}

void Detector::Fill(qdc_x3 value, int Time)
{
}

void Detector::Fill(qdc_x4 value, int Time)
{
}

void Detector::Fill(crrc4_spectro value, int Time)
{
}

void Detector::Fill(trapez_spectro value, int Time)
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