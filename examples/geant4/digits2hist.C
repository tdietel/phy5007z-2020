
void digits2hist()
{

  // ----------------------------------------------------------------------
  // set up data structures for reading

  // open main file
  TFile* f = new TFile("data/trddigits.root");

  // get `o2sim` tree from file
  TTree* o2sim = NULL;
  f->GetObject("o2sim", o2sim);


  // instantiate the reader for the tree
  TTreeReader reader(o2sim);

  // set up the branches we want to read
  // TTreeReaderValue<o2::dataformats::MCEventHeader> hdr(reader,"MCEventHeader.");
  // TTreeReaderArray<o2::trd::HitType> trdhits(reader, "TRDHit");
  TTreeReaderArray<o2::trd::Digit> trddigits(reader, "TRDDigit");

  // ----------------------------------------------------------------------


  TH1F* hadc = new TH1F("hadc", "ADC spectrum;ADC;N",1024,-0.5,1023.5);

  // ----------------------------------------------------------------------
  // event loop

  int evno = 0;
  while (reader.Next()) {

    cout << "Event " << evno << ": " << trddigits.GetSize() << " digits" << endl;

    // cout << trdhits.GetSize() << endl;
    //
    for(int idig=0; idig<trddigits.GetSize(); idig++) {
      auto dig = trddigits[idig];

      for (int t=0; t<30; t++) {
        hadc->Fill(dig.getADC()[t]);

      }
    }

    //   hq->Fill(hit.GetCharge());
    //
    //   float dx = ( hit.GetPos() - trdhits[ihit-1].GetPos() ).Mag2();
    //   hqdx->Fill(dx,hit.GetCharge());
    // }


 } // TTree entry / event loop

 hadc->Draw();

}
