
void digitsdraw(int det,int row)
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

  TH2F* padrow = new TH2F("padrow", "padrow",144,-0.5,143.5,30,-0.5,29.5);
  TH3F* hdet = new TH3F("hdet", "hdet;pad;row;tb",144,-0.5,143.5, 16,-0.5,15.5, 30,-0.5,29.5);

  // ----------------------------------------------------------------------
  // event loop

  int evno = 0;
  while (reader.Next()) {

    cout << "Event " << evno << ": " << trddigits.GetSize() << " digits" << endl;

    for(int idig=0; idig<trddigits.GetSize(); idig++) {
      auto dig = trddigits[idig];

      if (dig.getDetector() != det) continue;

      for (int t=0; t<30; t++) {

        hdet->Fill(dig.getPad(), dig.getRow(), t, dig.getADC()[t]);

        if (dig.getRow() == row) {
          padrow->Fill(dig.getPad(), t, dig.getADC()[t]);
        }
      }
    }

 } // TTree entry / event loop

 hdet->Draw("lego");
 // padrow->Draw("colz");

}
