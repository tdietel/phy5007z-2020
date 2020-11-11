
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

int DigitIdx(const o2::trd::Digit& a)
{
  int ret = a.getDetector();
  ret *= 1000; ret += a.getRow();
  ret *= 1000; ret += a.getPad();
  return ret;
}

bool DigitCompare(const o2::trd::Digit& a, const o2::trd::Digit& b)
{
  return DigitIdx(a) < DigitIdx(b);
}

// using DigitContainer = TTreeReaderArray<o2::trd::Digit>;
// using DigitContainter::iterator as DigitIterator;


// template<class T>
// class IteratorRange {
// public:
//   IteratorRange(TTreeReaderArray<T>::iterator start, int n) : begin(start), end(start+n) {};
//   TTreeReaderArray<T>::iterator begin,end;
// };

class DataManager
{

public:

  DataManager(std::string dir="data/")
  : fhits(0), fdigits(0), trhits(0), trdigits(0), rdrhits(0), rdrdigits(0),
    hits(0), digits(0), trgrecords(0),
    tfno(0), evno(0), iEvent(-1)
  {
    ConnectMCHitsFile(dir+"o2sim_HitsTRD.root");
    ConnectDigitsFile(dir+"trddigits.root");
  }

  bool NextTimeFrame()
  {
    if (rdrdigits->Next() ) {

      cout << "## Time frame " << tfno << endl;

      iEvent = 1;
      evno = 0;
      tfno++;
      return true;

    } else {
      return false;
    }

  }

  bool NextEvent()
  {
    // get the next trigger record
    if (iEvent >= trgrecords->GetSize()) {
      return false;
    }

    // load the hits for the next event
    if ( ! rdrhits->Next() ) {
      cout << "no hits found for event " << tfno << ":" << evno << endl;
      return false;
    }

    auto evrec = GetTriggerRecord();
    cout << evrec << endl;

    cout << "## Event " << tfno << ":" << evno << ":  "
         << hits->GetSize() << " hits   "
         << evrec.getNumberOfObjects() << " digits" << endl;

    event_digits.b = digits->begin() + evrec.getFirstEntry();
    event_digits.e = event_digits.begin() + evrec.getNumberOfObjects();
    std::stable_sort(event_digits.b,event_digits.e,DigitCompare);

    event_hits.b  = hits->begin();
    event_hits.e  = hits->end();

    evno++;
    iEvent++;
    return true;
  }

  o2::trd::TriggerRecord GetTriggerRecord() {return trgrecords->At(iEvent);}



  // A struct to be used for range-based for loops
  // A template would be nice, but I did not manage to make it work.
  struct DigitRange {
    TTreeReaderArray<o2::trd::Digit>::iterator& begin() { return b; }
    TTreeReaderArray<o2::trd::Digit>::iterator& end() { return e; }
    TTreeReaderArray<o2::trd::Digit>::iterator b, e;
   };

   DigitRange event_digits, padrow_digits;
   DigitRange Digits() {return event_digits;}

  // A struct to be used for range-based for loops
  // A template would be nice, but I did not manage to make it work.
  struct HitRange {
    TTreeReaderArray<o2::trd::HitType>::iterator& begin() { return b; }
    TTreeReaderArray<o2::trd::HitType>::iterator& end() { return e; }
    TTreeReaderArray<o2::trd::HitType>::iterator b, e;
   };

   HitRange event_hits;
   HitRange Hits() {return event_hits;}

   // DigitRange Digits(int det) {
   //   auto ret = Digits();
   //   while ( ret.b.getDetector() < det) { ret.b++; }
   //   while ( ret.e.getDetector() > det) { ret.e--; }
   //   return ret;
   // }

  // DigitRange Digits() {
  //   DigitRange ret;
  //   auto evrec = GetTriggerRecord();
  //   ret.b = digits->begin() + evrec.getFirstEntry();
  //   ret.e = ret.begin() + evrec.getNumberOfObjects();
  //
  //   // sort the digits for this event
  //   std::stable_sort(ret.b,ret.e,DigitCompare);
  //   return ret;
  // }

  // DigitRange Digits(int det, int row) {
  //
  //   // get a sorted list of digits
  //   auto ret = Digits();
  //
  //   while(ret.b->getDetector() < det) {
  //     ret.b++;
  //   }
  //
  //   while(ret.b->getDetector() == det || ret.b->getRow() < row ) {
  //     ret.b++;
  //   }
  //
  //   for ( auto& c = ret.b; c != ret.e; c++ ) {
  //     if c
  //   }
  //
  //   while( ret.b->getDetector() == det ret.b->getRow() < row ) {ret.b++;}
  //
  //
  //   // sort the digits for this event
  //
  //   return ret;
  // }

  void bla()
  {

    auto evrec = GetTriggerRecord();
    auto b = digits->begin() + evrec.getFirstEntry();
    auto e = b + evrec.getNumberOfObjects();

    cout << evrec << endl;
    // cout << b << endl;

    // for (auto dig = b; dig != e; dig++) {
    //   cout << (*dig) << endl;
    // }

    // auto d = Digits();
    for (auto& dig : Digits()) {
      cout << dig << endl;
    }
  }

protected:
  void ConnectMCHitsFile(std::string fname)
  {
    // ----------------------------------------------------------------------
    // set up data structures for reading

    if (fhits || trhits) {
      cerr << "Hits file seems to be connected." << endl;
      return;
    }

    fhits = new TFile(fname.c_str());
    fhits->GetObject("o2sim", trhits);

    rdrhits = new TTreeReader(trhits);
    hits = new TTreeReaderArray<o2::trd::HitType>(*rdrhits, "TRDHit");
  }

  void ConnectDigitsFile(std::string fname)
  {

    fdigits = new TFile(fname.c_str());
    fdigits->GetObject("o2sim", trdigits);

    rdrdigits = new TTreeReader(trdigits);

    // set up the branches we want to read
    digits = new TTreeReaderArray<o2::trd::Digit>(*rdrdigits, "TRDDigit");
    trgrecords = new TTreeReaderArray<o2::trd::TriggerRecord>(*rdrdigits, "TriggerRecord");
  }

private:
  TFile *fhits, *fdigits;
  TTree *trhits, *trdigits, *trtrgrec;
  TTreeReader *rdrhits, *rdrdigits, *rdrtrgreg;

  TTreeReaderArray<o2::trd::HitType>* hits;
  TTreeReaderArray<o2::trd::Digit>* digits;
  TTreeReaderArray<o2::trd::TriggerRecord>* trgrecords;

  int tfno, evno, iEvent;
};
