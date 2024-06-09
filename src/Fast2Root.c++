#include "Fast2Root.hh"

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
      filename_base = argv[1];
      reader = faster_file_reader_open(filename_base.c_str());
      faster_file_reader_close(reader);
      filename_base = filename_base.substr(0, filename_base.size() - 10);

      ifstream file(argv[1], ifstream::ate | ifstream::binary);
      file_size = static_cast<int>(file.tellg());
      file.close();

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

  /////////// ROOT FINAL FILE /////////////////////
  ROOTFile = new TFile((filename_base + ".root").c_str(), "RECREATE");

  /////////// BUILD DETECTORS /////////////////////
  map<string, ChannelInfo> channelInfoMap = InitDetectors(setupFile);

  /////////// BUILD TREE //////////////////////////
  #ifndef USE_SIGNAL_DICT
  Tree = new TTree("Tree", "Tree");
  Tree->Branch("Channel", &Channel);
  Tree->Branch("Time", &Time);
  Tree->Branch("Label", &Label);

  Tree_Group = new TTree("Tree_Group", "Tree_Group");
  Tree_Group->Branch("Channel", &Channel_vec);
  Tree_Group->Branch("Time", &Time_vec);
  Tree_Group->Branch("Label", &Label_vec);
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

  string arg = argv[1];
  size_t found = arg.find_last_of("_");
  string extracted = arg.substr(found + 1, 4);
  int subrun_number = stoi(extracted);
  string filename_base = arg.substr(0, found);

  string filename = argv[1];
  cout << endl;
  F2RInfo("Starting reading : " + string(gSystem->BaseName(filename_base.c_str())));

  reader = faster_file_reader_open(filename.c_str());
  
  while (reader != 0)
  {
    F2RInfo("SubFast file : " + to_string(subrun_number));

    while ((data = faster_file_reader_next(reader)) != NULL)
    {    
      coder = faster_data_type_alias(data);
      label = faster_data_label(data);
      clock_ns = faster_data_clock_ns(data);
      Filling();
    }
    faster_file_reader_close(reader);
    subrun_number++;
    stringstream ss;
    ss << setw(4) << setfill('0') << subrun_number;
    filename = filename_base + "_" + ss.str() + ".fast";
    reader = faster_file_reader_open(filename.c_str());
  }
  faster_file_reader_close(reader);
  F2RInfo("End of reading");
  

  ///////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////// WRITTING ROOT FILE /////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////
  F2RInfo("Writting ROOT file");
  WriteHistograms();
  
  Tree->Write();
  Tree_Group->Write();
  ROOTFile->Close();
  ///////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////// END OF PROGRAM /////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////

  return 0;
}
