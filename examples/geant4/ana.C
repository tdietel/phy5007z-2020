
void ana()
{

  // ----------------------------------------------------------------------
  // set up data structures for reading

  // open main file
  TFile* f = new TFile("data/o2sim_Kine.root");

  // get `o2sim` tree from file
  TTree* o2sim = NULL;
  f->GetObject("o2sim", o2sim);

  // add a friend branch for the hits
  o2sim->AddFriend("o2sim", "data/o2sim_HitsTRD.root");

  // instantiate the reader for the tree
  TTreeReader reader(o2sim);

  // set up the branches we want to read
  TTreeReaderValue<o2::dataformats::MCEventHeader> hdr(reader,"MCEventHeader.");
  TTreeReaderArray<o2::trd::HitType> trdhits(reader, "TRDHit");

  // ----------------------------------------------------------------------
  // Book output histograms

  TH1F* hq = new TH1F("hq", "TRD Hit Value;Q;entries",1000,-0.5,999.5);
  TH2F* hqdx = new TH2F("hqdx", "TRD Hit Value vs dx;dx;Q",
                        40,0.0,2.0, 50,-0.5,999.5);


  // ----------------------------------------------------------------------
  // event loop

  int evno = 0;
  while (reader.Next()) {

    cout << "Event " << evno << ": " << hdr->GetNPrim() << "primaries" << endl;

    cout << trdhits.GetSize() << endl;

    for(int ihit=1; ihit<trdhits.GetSize(); ihit++) {
      auto hit = trdhits[ihit];

      hq->Fill(hit.GetCharge());

      float dx = ( hit.GetPos() - trdhits[ihit-1].GetPos() ).Mag2();
      hqdx->Fill(dx,hit.GetCharge());
    }


 } // TTree entry / event loop

 // ----------------------------------------------------------------------
 // finalization: display and save results

 // hq->Draw();
 hqdx->Draw("colz");

}
