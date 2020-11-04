# Demo: Kalman Filter

This demo requires ROOT 6.

## Track simulation
First, simulate tracks with
```
root -b -q -l simtrack.C
```

## Linear fit
In the next step, you can fit a straight line to the tracks, neglecting any
scattering that might occur in the detector layers.
```
root linfit.C
```
The script will produce a histogram of the difference between the true original
direction of the track and the fit result.


## Kalman filter
And finally, you can run a Kalman filter for these tracks.
```
root kalman.C
```
The script will also produce a histogram of the difference between the true
original direction of the track and the fit result. Note that the standard
deviation is significantly smaller than for the linear fit.

## Plotting
There is a macro to plot the tracks, which I just copied in, but I have some
problems running it. Take it as a starting point if you want to try it.
