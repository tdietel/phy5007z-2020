
Git Repository for "Data Science for High-Energy Physics" (PHY5007Z) - 2020
===========================================================================

I will use this repository to distribute example code, lecture demos and
similar material.

The demos will be tested on `hep02`, where you should have an account. You
can also run them elsewhere, but some examples will require many prerequisites,
like ROOT, Pythia, Geant or the ALICE O2 software suite, which can be quite
difficult to install.

Usage
-----

### Initial clone of the repository

To get a working copy of this repository, (log into `hep02` and) clone it
to your computer:
```
ssh -l myusername -Y hep02.phy.uct.ac.za
git clone https://github.com/tdietel/phy5007z-2020.git
cd phy5007z-2020
```

### Staying up to date

When new content appears on GitHub, it will not automatically be synchronized
to your working directory.
```
git pull
```

If you have local changes, git will try to merge them. If this fails, git will
create a conflict that you have to resolve manually. You can also set up your
own branches, or fork the main repository.

There are many excellent git tutorials on the web. Please take a look at them,
and ask if you have any questions after going through them.

### Software environment: Pythia

Once the repository is set up, set up your software environment to run the
Pythia examples. On `hep02`, this can be done with a single command:
```
. /opt/hep/setup.sh
```

### Software environment: ALICE/O2

To set up the ALICE/O2 environment, source the o2env.sh script in the root
directory of this repository:
```
cd path/to/phy5007z-2020
. o2env.sh
```

### Jupyter Notebooks

Some of the examples are provided as Jupyter notebooks. To run these notebooks,
you will have to enable ssh port forwarding and run the notebook on a different
port (to avoid collisions with other users' notebook servers). In the examples
below, you should replace 1234 with a different number between 1024 and 65000.

Connect with local ssh port forwarding:
```
ssh -L 1234:localhost:1234
```

Source the ALICE/O2 environment and start a Jupyter Notebook:
```
cd path/to/phy5007z-2020
. o2env.sh
jupyter notebook --port 1234 --no-browser
```

Jupyter will display a URL. You have to copy and open in your browser on your
laptop.
