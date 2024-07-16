#include "Fast2Root.hh"
#include <unistd.h>

int main(int argc, char **argv)
{

  if (argc == 1)
  {
    F2RError("Please provide the name of the file to convert as argument");
    exit(0);
  }
  else
  {
    string input = argv[1];
    if (input.find(".fast") == string::npos)
    {
      F2RError("The file must be a .fast file");
      exit(0);
    }
    else
    {
      fast_filename_raw = argv[1];
      filename_base = argv[1];
      reader = faster_file_reader_open(filename_base.c_str());
      faster_file_reader_close(reader);
      filename_base = filename_base.substr(0, filename_base.size() - 10);

      F2RSuccess("Data file found");

      setupFile = filename_base + ".setup";
      ifstream infile(setupFile.c_str());

      if (infile.good())
      {
        ReadRunTime();
        F2RSuccess("Setup file found");
      }
      else
      {
        F2RError("Setup file not found : " + setupFile);
        exit(0);
      }
    }

    inputrange = "Range_Input.txt";
    ifstream inputfile(inputrange.c_str());

    if (inputfile.good())
    {
      F2RSuccess("Histogram range file found");
      HISTOGRAMS_RANGES = true;
    }
    else
    {
      F2RWarning("Range file not found : " + inputrange);
      HISTOGRAMS_RANGES = false;
    }
  }

  //////OPTION
  string outputPath;
  int opt;

  while ((opt = getopt(argc, argv, "o:")) != -1)
  {
    switch (opt)
    {
    case 'o':
      outputPath = optarg;
      if (!FolderExists(outputPath))
        {
          F2RError("Output path does not exist");
          exit(0);
        }
      break;
    default: 
      size_t lastDelimiterPos = filename_base.rfind('/');
      outputPath = filename_base.substr(0, lastDelimiterPos);
      exit(0);
    }
  }

  if (optind >= argc)
  {
    F2RError("Expected argument after options");
    exit(0);
  }

  /////////// ROOT FINAL FILE /////////////////////
  size_t lastDelimiterPos = filename_base.rfind('/');
  string filename_raw = filename_base.substr(lastDelimiterPos + 1);
  ROOTFile = new TFile((outputPath + filename_raw + ".root").c_str(), "RECREATE");

  /////////// BUILD DETECTORS /////////////////////
  map<string, ChannelInfo> channelInfoMap = InitDetectors(setupFile);

  /////////// BUILD TREE //////////////////////////
  #ifndef USE_SIGNAL_DICT
  Tree = new TTree("Tree", "Tree");
  Tree->Branch("Channel", &Channel);
  Tree->Branch("Time", &Time);
  Tree->Branch("Label", &Label);
  Tree->Branch("PileUp", &PileUp);

  Tree_Group = new TTree("Tree_Group", "Tree_Group");
  Tree_Group->Branch("Channel", &Channel_vec);
  Tree_Group->Branch("Time", &Time_vec);
  Tree_Group->Branch("Label", &Label_vec);
  Tree_Group->Branch("PileUp", &PileUp_vec);
  #endif

  #ifdef USE_SIGNAL_DICT
  Tree = new TTree("Tree", "Tree");
  Tree->Branch("Signal", &signal);

  Tree_Group = new TTree("Tree_Group", "Tree_Group");
  Tree_Group->Branch("Signal", &signal_vec);
  #endif

  ///////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////// READ FAST FILE ////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////

  size_t found = fast_filename_raw.rfind("_");
  string extracted = fast_filename_raw.substr(found + 1, 4);
  int subrun_number = stoi(extracted);
  string filename_base = fast_filename_raw.substr(0, found);

  size_t found_filename = fast_filename_raw.rfind("/");
  string fast_filename_short = fast_filename_raw.substr(0, found_filename);
  cout << endl;
  F2RInfo("Starting reading : " + fast_filename_short);

  reader = faster_file_reader_open(fast_filename_raw.c_str());
  
  while (reader != 0)
  {
    F2RInfo("SubFast file : " + to_string(subrun_number));

    while ((_data = faster_file_reader_next(reader)) != NULL)
    {    
      coder = faster_data_type_alias(_data);
      label = faster_data_label(_data);
      clock_ns = faster_data_clock_ns(_data);
      Filling();
    }
    faster_file_reader_close(reader);
    subrun_number++;
    stringstream ss;
    ss << setw(4) << setfill('0') << subrun_number;
    fast_filename_raw = filename_base + "_" + ss.str() + ".fast";
    reader = faster_file_reader_open(fast_filename_raw.c_str());
  }     

  faster_file_reader_close(reader);
  F2RInfo("End of reading");
  

  ///////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////// WRITTING ROOT FILE /////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////
  F2RInfo("Writting ROOT file");
  ROOTFile->cd();
  WriteHistograms();
  
  ROOTFile->cd();
  Tree->Write();
  Tree_Group->Write();
  pair<string, string> FasterTime = ReadRunTime();
  TNamed("Start_Time", FasterTime.first.c_str()).Write();
  TNamed("End_Time", FasterTime.second.c_str()).Write();
  ROOTFile->Close();
  ///////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////// END OF PROGRAM /////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////

  return 0;
}
