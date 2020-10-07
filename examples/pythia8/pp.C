/// pythia8 basic example
///
/// to run, do:
///
/// ~~~{.cpp}
///  root > .x pythia8.C
/// ~~~
///
/// Note that before executing this script,
///
///  - the env variable PYTHIA8 must point to the pythia8100 (or newer) directory
///  - the env variable PYTHIA8DATA must be defined and it must point to $PYTHIA8/xmldoc
///
/// \macro_code
///
/// \author Andreas Morsch

void pp(Float_t energy = 10000., Int_t nev  = 100, Int_t ndeb = 1)
{
  gROOT->Macro("loadPythia.C");
  //loadPythia();

  // Histograms
  //TH1F* etaH = new TH1F("etaH", "Pseudorapidity", 120, -12., 12.);
  //TH1F* ptH  = new TH1F("ptH",  "pt",              50,   0., 10.);

  // Array of particles
  TClonesArray* particles = new TClonesArray("TParticle", 1000);
  // Create pythia8 object
  TPythia8* pythia8 = new TPythia8();
  
  // Configure
  //pythia8->ReadString("PromptPhoton:all = on");
  pythia8->ReadString("WeakSingleBoson:all = on");
  pythia8->ReadString("WeakDoubleBoson:all = on");
  //pythia8->ReadString("WeakBosonAndParton:all = on");
  pythia8->ReadString("Onia:all = on");
  //pythia8->ReadString("Bottomonium:all = on");
  pythia8->ReadString("SoftQCD:all = on");
  pythia8->ReadString("HardQCD:all = on");
  pythia8->ReadString("HardQCD:gg2bbbar = on");
  pythia8->ReadString("HardQCD:qqbar2bbbar = on");
  
  // Initialize
  pythia8->Initialize(2212 /* p */, 2212 /* p */, energy /* GeV */);
  
  // Event loop
  for (Int_t iev = 0; iev < nev; iev++) {
    pythia8->GenerateEvent();
    if (iev < ndeb) pythia8->EventListing();
  }
  
  pythia8->PrintStatistics();

}
