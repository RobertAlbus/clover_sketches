
bisect: identified where copy patch for kick drum stopped working
64cffc91e60a08c0ed37b9e2cb531ea92d8a6c48

----

- std::memory_order_seq_cst in atomic and now the change is detected across threads
    - std::memory_order_release on set, std::memory_order_acquire on release

## useful utils
- debugging table:
    - easy to use table for displaying debug values with headings and lefthand labels
    - maybe not, at least not now
    - cache coherency issue solved 
