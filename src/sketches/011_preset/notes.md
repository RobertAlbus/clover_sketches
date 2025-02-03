- the system was blowing up because the fb coefficient on the resonator was dumb
- need to tune the fb coefficients better (both the resonator and the fdn decay)
- currently not blowing up but also not hearing any reverb


-- HEY the resonator has feedback built in but the fdn is also feeding back. 
--- don't need both.
--- rework things a bit here...
- just need:


delay --------> hadamard --------> out
 ^- fbg <- hp <- lp -/

