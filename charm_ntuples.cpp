#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include <vector>
#include <chrono>
#include <string>
#include <highfive/H5File.hpp>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>

using namespace std;
HighFive::File file("Sherpa_Zjets_output_all_entries.h5", HighFive::File::ReadWrite | HighFive::File::Create | HighFive::File::Truncate);

void tree1rw(const char*  branchname) {
    //read the Tree 
    //note that we use "new" to create the TFile and TTree objects because we want to keep these objects alive when we leave this function.
    TFile *f = new TFile("Sherpa_Zjets.MC16e.364200.root");
    TTree *t1 = (TTree*)f->Get("CharmAnalysis");
    float branch;
    t1->SetBranchAddress(branchname, &branch);
    
    vector<float> data;
    int nentries = t1->GetEntries();

    for (int i=0;i<nentries;i++) {
        t1->GetEntry(i);
        data.push_back(branch);
    }
    f->Close();

    // let's create a dataset of native integer with the size of the vector
    // 'data'
    
    HighFive::DataSet dataset = file.createDataSet<float>(branchname, HighFive::DataSpace::From(data));
    // let's write our vector of int to the HDF5 dataset
    dataset.write(data);
} 
int main()
{
    string  branches[27] = {"CharmEventInfo_passedElectronChain", "CharmEventInfo_passedMuonChain","CharmEventInfo_TIGHT_EL1_PT", "CharmEventInfo_TIGHT_EL2_PT", "CharmEventInfo_TIGHT_MU1_PT", "CharmEventInfo_TIGHT_MU2_PT", "CharmEventInfo_TIGHT_EL1_MATCHED", "CharmEventInfo_TIGHT_EL2_MATCHED", "CharmEventInfo_TIGHT_MU1_MATCHED", "CharmEventInfo_TIGHT_MU2_MATCHED", "CharmEventInfo_MET", "CharmEventInfo_METPhi", "CharmEventInfo_TIGHT_EL1_PHI", "CharmEventInfo_TIGHT_EL1_ETA", "CharmEventInfo_TIGHT_EL1_CHARGE", "CharmEventInfo_TIGHT_EL2_PHI", "CharmEventInfo_TIGHT_EL2_ETA", "CharmEventInfo_TIGHT_EL2_CHARGE", "CharmEventInfo_TIGHT_MU1_PHI", "CharmEventInfo_TIGHT_MU1_ETA", "CharmEventInfo_TIGHT_MU1_CHARGE", "CharmEventInfo_TIGHT_MU2_PHI", "CharmEventInfo_TIGHT_MU2_ETA", "CharmEventInfo_TIGHT_MU2_CHARGE", "CharmEventInfo_EventWeight", "CharmEventInfo_TIGHT_EL_WEIGHT", "CharmEventInfo_TIGHT_MU_WEIGHT"};
//  auto start = std::chrono::system_clock::now();
    for (int i = 0; i < 27; i++) { 
	tree1rw((branches[i]).c_str());
      }
    cout << "done \n";
//  auto end = std::chrono::system_clock::now();
//  std::chrono::duration<double> diff = end-start;
//  printf("read/write time: %8f \n", diff.count());
}
