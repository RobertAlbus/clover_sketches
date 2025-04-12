# Reflections: 019_composition

--------------------------------
## Sequencing and Meta Sequencing are bulky
- lots of repetition and boilerplate
    - hard to find errors if I miss changes during copy-paste setup
- template specialization for `frsq` makes it cumbersome

**ideas**
- next point **Pattern events are not linked to pattern durations** could help with this
    - less setup if patterns contain abs/rel durations
- meta_sequence the `sequencer` struct, not individual `frsq` instances?
    - `struct sequencer   { /**/ };` operates on instruments
    - `struct arrangement { /**/ };` operates on `sequencer`
- `frsq` could treat voices and events as `void*`
    - cast to correct type in the callback
    - would reduce compilation times
    - could this improve runtime performance?
        - does a higher number of template specializations reduce runtime performance for any reason?


--------------------------------
## Pattern events are not linked to pattern durations
The sequencer currently owns the abs/rel durations. The event list for the pattern must adhere to this.

### Potential solution
- define concept(s) and struct(s) that contain all pattern metadata
    - `pattern_concept` that requires abs/rel durations
- implement patterns as structs that


--------------------------------
## No transport control
- cannot start the composition part way though
- meta `frsq` can be started part way through, but not propagating this to `frsq` instances
- automation clips created with `env_bp` don't handle arbitrary starting positions


--------------------------------
## need better UI coverage
- not all instruments have a UI that I can tweak
- having UI will make it easier to tweak patches


--------------------------------
## need better UI coverage



--------------------------------
## Compile-Time Patterns and Patches
Patterns and patches are set up as the definitions of members in structs.
This was initially chosen because it seemed like it would be simpler to implement.

**Recompilation of unrelated units**
Changing a synth patch for one instrument causes recompilation of several unrelated compilation units.

```sh
[ 95%] Building CXX object src/sketches/019_composition/CMakeFiles/019_composition.dir/composition/composition.cpp.o
[ 95%] Building CXX object src/sketches/019_composition/CMakeFiles/019_composition.dir/main.cpp.o
[ 96%] Building CXX object src/sketches/019_composition/CMakeFiles/019_composition.dir/sequence/sequencers.cpp.o
[ 96%] Building CXX object src/sketches/019_composition/CMakeFiles/019_composition.dir/views/views.cpp.o
```

**Need to instantiate the containing structs**
```c++
// declaration
struct composition {
    ...
    static patch_drums_t patch_drums;
    static patch_synth_t patch_synth;
    static patch_mix_t mix;
};

// definition
patch_drums_t composition::patch_drums{};
patch_synth_t composition::patch_synth{};
patch_mix_t composition::mix{mix_patch};
```

### Alternatives
**factory/builder pattern for creating patches and/pr patterns**
- opens the door to procgen with builder arguments
```c++
nx_osc_props create_pad_patch() {
    return nx_osc_props{ /* props */ };
}

fdn_8_props_019 create_tuned_resonator_fdn(
    float lpf_f0, float lpf_q,
    float hpf_f0, float hpf_q,
    float f1, float f2, float f3, float f4,
    float f5, float f6, float f7, float f8) {
        return fdn_8_props_019{ /* props */ };
    }


```

**namespaced objects**
- `extern` declaration in header
- definition in source file

--------------------------------
## objects not updating from props struct
- props structs are pocos that contains scalars
- dsp object doesn't know when props change
    - currently I either don't update at all, or update every sample
- would like as-needed updates of only changed settings

--------------------------------
## Always-on processing
Do not currently have a method to determine if an object needs to be processed or can be skipped




