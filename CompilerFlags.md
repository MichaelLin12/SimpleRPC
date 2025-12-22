# Flags for GCC

* -O0 
    * the compiler performs only the minimal work needed to compile code
    * fast compilation time, accurate debugging, generated code is as close as possible to source code
    * no inlining, no loop unrolling, no vectorization ... and generally minimal optimizations
* -O2
* -O3
* -fsanitize=address 
    * enable the Address sanitizer ASAN
    * detect heap buffer overflow/underflow, stack buffer overflow/underflow, use-after-scope, use after free, global and static memory overflow and underflow
    * detects invalid pointer arithmetic leading to memory access outside allocated bounds.
    * only works on code compiled and linked with these flags
* -fsanitize=leak
* -fsanitize=memory
* -fsanitize=undefined
* -fsanitize=thread
* -g 
    * also -g2
    * normal debug info. Show lines numbers, variables, types
* -g3
    * maximum debug info. Incllude macro definitions in addition to everything -g2 provides
    * provides full type info
    * improves GDB's ability to inspect variables and constants
* -Wall 
    * enable a common set of compiler warnings
* -Wextra 
    * enable warnings not included in the common set
    * focus on potentially risky or unusual code patterns
* -Wpedantic
    * enforce strict compliance with C/C++ standard
    * warns about anything the standard considers undefined, implementation-defined, or nonstandard
* -Wshadow 
    * warns when a local variaable or function parameter shadows another variable in an outer scope
* -Wconversion 
    * warns about implicit type conversions that might change a value
* -Wsign-conversion 
    * warns about implicit conversions between signed and unsigned types
* -Werror
    * treat all warnings as errors
* -fopt-info-optimization
    * reports only optimizations that the compiler made to your code
* -march=native
    * generate cpu-specific instructions
* -flto
    * perform optimizations at link time across translation units
    * reduces binary size
    * may help with hot path performance in the I-cache
* -fuse-linker-plugin
    * enables full interprocedural optimization accross all translation units at link time
    * often results in faster linking and smaller, better optimized binaries