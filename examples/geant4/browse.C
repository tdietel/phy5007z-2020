

void browse()
{
  TFile *fkine = new TFile("data/o2sim_Kine.root");
  TFile *fhtrd = new TFile("data/o2sim_HitsTRD.root");
  TFile *fhtpc = new TFile("data/o2sim_HitsTPC.root");
  TFile *fhits = new TFile("data/o2sim_HitsITS.root");

  new TBrowser();
}
