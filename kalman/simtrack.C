
void simtrack(int nevents=10000)
{

  ofstream fout("tracks.txt");

  TRandom3 rnd;

  for (int ev=0; ev<nevents; ev++) {

    double incl = rnd.Uniform(-1., 1.);
    double incl_curr = incl;

    double ytrack = 0;

    fout << incl << " ";

    for (int i=1; i<=10; i++) {

      // the point moves by current inclination
	    ytrack += incl_curr;

	    // the measurement of the point has a resolution
	    double ymeas = ytrack + rnd.Gaus(0., 0.1);

	    // in the measurement station, the inclination of the track is
	    // changed by 0.02
	    incl_curr += rnd.Gaus(0., 0.1);

	    fout << ymeas << " ";
    }

    fout << endl;
  }
}
