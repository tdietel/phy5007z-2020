
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

From here on, I will assume that you are in this directory.

### Software environment

Once the repository is set up, set up your software environment. On `hep02`,
this can be done with a single command:
```
. /opt/hep/setup.sh
```

As of now, this sets up ROOT and Pythia8 for the first exercises. More
components, namely Geant and ALICE O2 will follow.

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
