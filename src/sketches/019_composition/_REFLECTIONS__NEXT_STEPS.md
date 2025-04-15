# Reflections: next steps

--------------------------------
## composition template
- duplicate 019_composition and strip back to boilerplate
- copy into 000_template
- don't worry about fixing too much
    - main win would be avoiding structs for aggregating patches and patterns.

--------------------------------
## spend time playing
attending to the friction points in the compositional process could be a large amount of work.
I want to spend some time playing around and exploring ideas before taking that on.
- build excitement and anticipation for solving the worst friction points.
- enjoy the fruits of my labor up to this point
- embrace slow productivity

things I want to explore:
- more resonator chords
- cymbal washes
- dub delays
- procgen patterns and euclidean rhythms
- groove templates
- real time pattern lerp
- FDN modulation
- FDN with different feedback processing
- colorless FDN

highest priority refactors:
- props are patches, not inputs
    - create setter methods on the ugens that can be called in the main event loop or from UI
    - consider a separate instantiation of the props for the GUI
- patches and patterns should be their own compilation unit
    - the current approach leads increased compile times because patches and patterns are header-only.
    - fine to keep them in a struct, but I need to...
    - explicitly instantiate the struct in it's own copmpilation unit


