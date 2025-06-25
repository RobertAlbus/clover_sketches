# 031_reso_snare

## design
```
impulse signal -> body resonator -> distortion -> reverb 
        "------------------------------------------^
```

- signal flow could be improved
- initially used cymbal instrument tuned "wrong" as impulse
    - simple stereo noise sounds better
    - true noise creates unreliable transient, recorded noise would help this
    - maybe still use square waves, just fewer, without funny cymbal filters

## main take aways
- reverb not really needed
- I need visual metering! hard to dial in drive coefficients for distortion
- gui for subtractive synth will be useful for designing impulses
- stereo really helps with being beefy

## next attempt ideas
- noise or simple square impulse
- better signal routing
- light distortion in feedback path?
- additional freq-selective damping
- fewer delay lines? e.g. fdn2 or fdn4
