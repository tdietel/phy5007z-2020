void plot(char *infile = "tracks.txt")
{
  ifstream fin(infile);

  TCanvas* c1 = new TCanvas();

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


    //cout << incl << " " << gr.GetFunction("pol1")->GetParameter(1) << endl;

    gr.Draw("AC*");

    c1->Modified();
    c1->Update();

    int foo;
    cin >> foo;

    //TSystem::Sleep(1);
  }

}
