
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
