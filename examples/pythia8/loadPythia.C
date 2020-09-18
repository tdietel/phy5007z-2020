
void loadPythia()
{
  const char *p8dataenv = gSystem->Getenv("PYTHIA8DATA");
  if (!p8dataenv) {
    const char *p8env = gSystem->Getenv("PYTHIA8");
    if (!p8env) {
      Error("pythia8.C",
	    "Environment variable PYTHIA8 must contain path to pythia directory!");
      return;
    }
    TString p8d = p8env;
    p8d += "/xmldoc";
    gSystem->Setenv("PYTHIA8DATA", p8d);
  }

  const char* path = gSystem->ExpandPathName("$PYTHIA8DATA");
  if (gSystem->AccessPathName(path)) {
    Error("pythia8.C",
	  "Environment variable PYTHIA8DATA must contain path to $PYTHIA8/xmldoc directory !");
    return;
  }
  
  // Load libraries
  if (gSystem->Getenv("PYTHIA8")) {
    gSystem->Load("$PYTHIA8/lib/libpythia8");
  } else {
    gSystem->Load("libpythia8");
  }
  gSystem->Load("libEG");
  gSystem->Load("libEGPythia8");

}
