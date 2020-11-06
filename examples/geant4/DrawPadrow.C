
// #include <>

std::ostream& operator<<(std::ostream& os, const o2::trd::Digit& d)
{
  int tbsum=0;
  auto adcs = d.getADC();

  for (int i=0;i<30;i++) {
    tbsum += adcs[i];
  }

  os << d.getDetector() << ":" << d.getRow() << ":" << d.getPad()
     << "  tbsum=" << tbsum;

  return os;
}

std::ostream& operator<<(std::ostream& os, const o2::trd::HitType& h)
{
  os << h.GetDetectorID() << ":"
     << h.getLocalC() << "/" << h.getLocalT() << "/" << h.getLocalR()
     << "  q=" << h.GetCharge();

  return os;
}


class SpacePointConverter
{
private:
  o2::trd::TRDGeometry* geo;

public:
  SpacePointConverter() {
    geo = o2::trd::TRDGeometry::instance();
    geo->createPadPlaneArray();
  }

  array<double,3> Hit2RowColTime(const o2::trd::HitType& hit)
  {
    return Local2RowColTime(hit.GetDetectorID()*2,
                            hit.getLocalT(), hit.getLocalC(), hit.getLocalR());
  }


  array<double,3> Local2RowColTime(int hcid, double x, double y, double z)
  {

    double vDrift = 1.5625; // in cm/us
    double t0 = 4.0; // time (in timebins) of start of drift region

    auto padPlane = geo->getPadPlane((hcid/2)%6, (hcid/12)%5);

    array<double,3> rct;

    double iPadLen = padPlane->getLengthIPad();
    double oPadLen = padPlane->getLengthOPad();
    int nRows = padPlane->getNrows();

    double lengthCorr = padPlane->getLengthOPad()/padPlane->getLengthIPad();

    // calculate position based on inner pad length
    rct[0] = - z / padPlane->getLengthIPad() + padPlane->getNrows()/2;

    // correct row for outer pad rows
    if (rct[0] <= 1.0) {
      rct[0] = 1.0 - (1.0-rct[0])*lengthCorr;
    }

    if (rct[0] >= double(nRows-1)) {
      rct[0] = double(nRows-1) + (rct[0] - double(nRows-1))*lengthCorr;
    }

    // sanity check: is the padrow coordinate reasonable?
    if ( rct[0]<0.0 || rct[0]>double(nRows) ) {
      cout << "ERROR: hit with z=" << z << ", padrow " << rct[0]
           << " outside of chamber" << endl;
    }

    // simple conversion of pad / local y coordinate
    // ignore different width of outer pad
    rct[1] = y / padPlane->getWidthIPad() + 144./2.;

    // time coordinate
    if (x<-0.35) {
      // drift region
      rct[2] = t0 - (x+0.35) / (vDrift/10.0);
    } else {
      // anode region: very rough guess
      rct[2] = t0 - 1.0 + fabs(x);
    }

    return rct;
  }
};

void DrawPadrow(int det,int row)
{

  // ----------------------------------------------------------------------
  // set up data structures for reading

  TFile* fhits = new TFile("data/o2sim_HitsTRD.root");

  // get `o2sim` tree from file
  TTree* trhits = NULL;
  fhits->GetObject("o2sim", trhits);

  // instantiate the reader for the tree
  TTreeReader reader_hit(trhits);

  // set up the branches we want to read
  TTreeReaderArray<o2::trd::HitType> trdhits(reader_hit, "TRDHit");

  // open main file
  TFile* fdig = new TFile("data/trddigits.root");

  // get `o2sim` tree from file
  TTree* trdig = NULL;
  fdig->GetObject("o2sim", trdig);


  // instantiate the reader for the tree
  TTreeReader reader_dig(trdig);

  // set up the branches we want to read
  TTreeReaderArray<o2::trd::Digit> trddigits(reader_dig, "TRDDigit");
  TTreeReaderArray<o2::trd::TriggerRecord> trdtrgrec(reader_dig, "TriggerRecord");

  SpacePointConverter conv;


  // ----------------------------------------------------------------------

  TH2F* padrow = new TH2F("padrow", ";pad;time bin",144,0.,144.,30,0.,30.);
  // TH2F* padrow = new TH2F("padrow", ";pad;time bin",144,-0.5,143.5,30,-0.5,29.5);
  // TH3F* hdet = new TH3F("hdet", "hdet;pad;row;tb",144,-0.5,143.5, 16,-0.5,15.5, 30,-0.5,29.5);

  TCanvas* cnv = new TCanvas("cnv_padrow", "cnv_padrow", 800,600);

  TMarker m;
  m.SetMarkerStyle(20);


  // ----------------------------------------------------------------------
  // event loop

  int evno = 0;
  int tfno = 0;

  // loop over the entries in trddigits.root, which are time frames
  while ( reader_dig.Next() ) {

    cout << "## Time frame " << tfno << endl;

    // loop over the trigger records - skip the first one (known bug)
    for(int iev=1; iev<trdtrgrec.GetSize(); iev++) {
      auto evrec = trdtrgrec[iev];

      cout << evrec << endl;

      // load the hits for the next event
      if ( ! reader_hit.Next() ) {
        cout << "no hits found for event " << tfno << ":" << evno << endl;
        break;
      }

      cout << "## Event " << tfno << ":" << evno << ":  "
           << trdhits.GetSize() << " hits   "
           << evrec.getNumberOfObjects() << " digits" << endl;

      // if (dig.getDetector() != det) continue;

      padrow->Clear();

      for(int idig=0; idig<evrec.getNumberOfObjects(); idig++) {
        auto dig = trddigits[idig+evrec.getFirstEntry()];

        if (dig.getDetector() != det) continue;

        cout << dig << endl;

        auto adc = dig.getADC();

        for (int i=0;i<30;i++) {
          padrow->Fill(dig.getPad(), i, adc[i]);
        }
      }

      padrow->Draw("colz");


      for(int ihit=0; ihit<trdhits.GetSize(); ihit++) {
        auto hit = trdhits[ihit];

        if (hit.GetDetectorID() != det) continue;

        auto rct = conv.Hit2RowColTime(hit);

        cout << hit << "   " << rct[0] << ":" << rct[1] << ":" << rct[2] << endl;

        m.DrawMarker(rct[1], rct[2]);
      }

      // cnv->Draw();
      // cnv->Update();
      // // cnv->Updated();
      // cnv->ForceUpdate();
      // cout << "sleeping..." << endl;
      // sleep(10);

      evno++;
    } // event/trigger record loop

    tfno++;
  } // time frame loop

 // hdet->Draw("lego");
 // padrow->Draw("colz");

}
