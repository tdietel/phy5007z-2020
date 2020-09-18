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

void pp_Upsilon(Float_t energy = 10000., Int_t nev  = 100, Int_t ndeb = 1)
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
  //pythia8->ReadString("WeakSingleBoson:all = on");
  //pythia8->ReadString("WeakDoubleBoson:all = on");
  //pythia8->ReadString("WeakBosonAndParton:all = on");
  //pythia8->ReadString("Onia:all = on");
  pythia8->ReadString("Bottomonium:all = on");
  //pythia8->ReadString("SoftQCD:all = on");
  //pythia8->ReadString("HardQCD:all = on");
  pythia8->ReadString("HardQCD:gg2bbbar = on");
  pythia8->ReadString("HardQCD:qqbar2bbbar = on");
  
  // Initialize
  pythia8->Initialize(2212 /* p */, 2212 /* p */, energy /* GeV */);
  
  Int_t pdgUpsilon=TDatabasePDG::Instance()->GetParticle("Upsilon")->PdgCode();
   
  Int_t nUpsilon = 0;
   
  // Event loop
  for (Int_t iev = 0; iev < nev; iev++) {
    pythia8->GenerateEvent();
    if (iev < ndeb) pythia8->EventListing();
    pythia8->ImportParticles(particles,"All");
    Int_t np = particles->GetEntriesFast();
    // Particle loop
    for (Int_t ip = 0; ip < np; ip++) {
      
      TParticle* part = (TParticle*) particles->At(ip);
      
      //Int_t ist = part->GetStatusCode();
      // Positive codes are final particles.
      //if (ist <= 0) continue;
      
      // ignore everything but Upsilons
      if (part->GetPdgCode() != pdgUpsilon) continue;
      
      nUpsilon++;
      cout << "found " << nUpsilon << " Upsilons" << endl; 
      
      //Int_t pdg = part->GetPdgCode();
      // Float_t charge = TDatabasePDG::Instance()->GetParticle(pdg)->Charge();
      
      
      //if (charge == 0.) continue;
      //Float_t eta = part->Eta();
      //Float_t pt  = part->Pt();
      //
      //etaH->Fill(eta);
      //if (pt > 0.) ptH->Fill(pt, 1./(2. * pt));
    }
  }
  
  pythia8->PrintStatistics();

//  double sigmaTot = pythia8->Pythia8()->info.sigmaGen();
//  double sigmaErr = pythia8->Pythia8()->info.sigmaErr();
//
//  
//  cout << "Total cross section: " << sigmaTot << " +/- " << sigmaErr << endl;
//
//  cout << "Upsilon cross section: "
//       << sigmaTot*double(nUpsilon)/double(nev) << " +/- "
//       << sigmaErr*double(nUpsilon)/double(nev) << endl;
//
//  
  
//   TCanvas* c1 = new TCanvas("c1","Pythia8 test example",800,800);
//   c1->Divide(1, 2);
//   c1->cd(1);
//   etaH->Scale(5./Float_t(nev));
//   etaH->Draw();
//   etaH->SetXTitle("#eta");
//   etaH->SetYTitle("dN/d#eta");
//
//   c1->cd(2);
//   gPad->SetLogy();
//   ptH->Scale(5./Float_t(nev));
//   ptH->Draw();
//   ptH->SetXTitle("p_{t} [GeV/c]");
//   ptH->SetYTitle("dN/dp_{t}^{2} [GeV/c]^{-2}");
}
