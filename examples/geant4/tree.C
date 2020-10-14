
void tree()
{

  TTree* o2sim = NULL;
  TFile* f = new TFile("data/o2sim_Kine.root");
  f->GetObject("o2sim", o2sim);

  o2sim->AddFriend("o2sim", "data/o2sim_HitsTRD.root");

  cout << o2sim->GetEntries() << endl;

  TCanvas* c1 = new TCanvas("MC truth pT spectrum");
  c1->SetLogy();
  o2sim->Draw("MCTrack.GetPt()");

  TCanvas* c2 = new TCanvas("TRD hit locations");
  o2sim->Draw("locT:locC");

  // o2sim->Print();

  cout << "Try it yourself:" << endl;
  cout << "   o2sim->Print()" << endl;
  cout << "   o2sim->Draw(\"mHitValue\")" << endl;


}
