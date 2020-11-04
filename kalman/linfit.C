void linfit(char *infile = "tracks.txt")
{
  ifstream fin(infile);

  TNtuple *fp = new TNtuple("fp","fp","incl:p0:p1:e0:e1");

  while(true) {

    double incl;
    double x[10], y[10];

    fin >> incl;

    for (int i=0; i<10; i++) {
      x[i] = i;
      fin >> y[i];
    }

    if (fin.eof()) break;

    TGraph gr(10,x,y);
    gr.Fit("pol1");

    fp->Fill(incl,
      gr.GetFunction("pol1")->GetParameter(0),
      gr.GetFunction("pol1")->GetParameter(1),
      gr.GetFunction("pol1")->GetParError(0),
      gr.GetFunction("pol1")->GetParError(1));

      //cout << incl << " " << gr.GetFunction("pol1")->GetParameter(1) << endl;

      //gr.Draw("AC*");

      //TSystem::Sleep(1);
    }

    fp->Draw("incl-p1");

}
