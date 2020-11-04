
#include <iomanip>

void kalman(char *infile = "tracks.txt")
{
  ifstream fin(infile);

  TNtuple *fp = new TNtuple("fp","fp","incl:y:m:c00:c01:c10:c11");

  const int nhits = 10;

  // measurement (detector) resolution
  float sigma_meas = 0.1;

  // process noise: scattering angle
  float sigma_process = 0.1;

  while(true) {

    double incl;
    double x[nhits], y[nhits];

    fin >> incl;

    for (int i=0; i<nhits; i++) {
      x[i] = i;
      fin >> y[i];
    }

    if (fin.eof()) break;


    TVectorD state(2);
    state[0] = y[nhits-1];
    state[1] = y[nhits-1] / float(nhits);

    TMatrixD C(2,2);
    C[0][0] = sigma_meas * sigma_meas; // detector resolution
    C[0][1] = 0.;
    C[1][0] = 0.;
    C[1][1] = 100.; // very large

    // transport model
    TMatrixD F(2,2);
    F[0][0] =  1.;
    F[0][1] = -1.;
    F[1][0] =  0.;
    F[1][1] =  1.;

    TMatrixD FT(2,2);
    FT.Transpose(F);

    // measurement matrix
    TMatrixD H(1,2);
    H[0][0] = 1.;
    H[0][1] = 0.;

    TMatrixD HT(2,1);
    HT.Transpose(H);

    // process noise covariance matrix
    TMatrixD Q(2,2);
    Q[0][0] = 0.;
    Q[0][1] = 0.;
    Q[1][0] = 0.;
    Q[1][1] = sigma_process * sigma_process;

    // measurement noise covariance matrix
    TMatrixD R(1,1);
    R[0][0] = sigma_meas * sigma_meas;

    // unit matrix
    TMatrixD Unit(2,2);
    Unit[0][0] = 1.;
    Unit[0][1] = 0.;
    Unit[1][0] = 0.;
    Unit[1][1] = 1.;

    for (int i=0; i<10; i++) {


      // Prediction
      TVectorD pred  = F * state;
      TMatrixD predC = F * C * FT + Q;

	    // calculate residual vector (1D) between measurement and prediction
	    // + covariance
	    TVectorD meas(1);
	    meas[0] = y[nhits - 1 - i];


      TVectorD res   = meas - H * pred;
	    TMatrixD resC  = H * predC * HT + R;

	    // invert the covariance matrix of the residuals
	    TMatrixD invResC(1,1);
	    invResC[0][0] = 1. / resC[0][0];

	    // Kalman gain: weighting factor for prediction VS measurement
	    TMatrixD kalman_gain = predC * HT * invResC;

	    // update state vector
	    state = pred + kalman_gain * res;
	    C = (Unit - kalman_gain * H) * predC;

    }


    fp->Fill(incl, state[0], state[1], C[0][0], C[0][1], C[1][0], C[1][1]);
  }

  fp->Draw("incl-m");
}
