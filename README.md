# What is this?

A non-practical solution to the problem of finding five English words with 25 distinct characters, using the C++ `constexpr` technique, in pursuit of the fastest execution time.

This is a fork of the algorithm authored by @ilyanikolaevsky, but forced into a C++ `constexpr`/`consteval` constrains, which resulted in the execution time around 200µs.

# Reasoning behind this project

The code was an experiment induced by this video: https://youtu.be/c33AZBnRHks.
The base algorithm chosen for conversion to the `constexpr` mode must have been using standard C++ operations (no third party libraries, no compilator-specific extensions), must have been single threaded and must have been relatively fast in execution.

This project should produce the binary with - most probably - the fastest **execution time** in the world. The code is arranged in such way that all the computations are done in the **compile time**. All the application has to do in the runtime, is to read and push the compile-time-calculated data to the file.

Does this makes sense from any practical point of view? It could have, but at this moment - absolutely not. To achieve such minimal execution time, the compile time is humungus: on my Ryzen 7 3700X system it takes around 345 minutes! Moreover - this is achievable only using proper compiler - clang has gave up at the point before even `can_construct_2` was fully calculated. This project should be considered as some kind of proof of concept rather than anything useful.

![meme of Homer comparing execution time vs compilation time](meme.jpg)

# Requirements

This code at this moment couldn't be compiled with clang. The only compiler which allows to do such big job at compile time is GCC. In order to prepare input data to be processed at compile time, a small trick with generating an intermediate input file must be done via CMake, thus please don't ignore the first step from the "Building and running" section.

Requirements:
- GCC 12 (earlier versions were not tested, thus I don't recommend trying - unless your machine has some working hours to waste; that being said - earlier versions could work, as I abandoned pretty much all newest constexpr stuff during the evolution of this experiment),
- CMake,
- make,
- recommended at least 16 GiB of RAM,
- Linux environment (other OSes should be also ok, as long as you would have available GCC and CMake; building instructions were written using Ubuntu).

# Building and running

First, use CMake to generate the intermediate input file and makefile:

```sh
mkdir build_constexpr
cd build_constexpr
cmake -DCMAKE_BUILD_TYPE=Release ..
```

Now you're ready to fire the compilation torture. Grab your favorite cup of coffee, cast the making spell and watch the dead terminal for about five to six hours:

```sh
make constexpr
```

The compilation process is single threaded - it has to be sequential, as later computations depend on the earlier ones, so throwing more jobs (with `make -j500 constexpr`) will make no difference.

This will produce the `constexpr` executable. Measuring its performance via `time` command is starting to be pointless, given the low precision, so the binary has built-in timer for measuring its own execution time within the nanoseconds' resolution. The output of the application will be saved to the `solutions.txt` file.

An example output from the application run:

```ShellSession
draghan@fortress:~/programming/five_words_constexpr/build_constexpr$ ./constexpr
Total time:   194269ns (194us)

draghan@fortress:~/programming/five_words_constexpr/build_constexpr$ head solutions.txt && echo ... && tail solutions.txt && cat ./solutions.txt | wc
bawke fldxt gconv jimpy qursh
bejig fldxt nymph quack vrows
evang fldxt jumby qophs wrick
exptl fconv gawby hdqrs mujik
exptl gconv hdqrs jumby wakif
expwy flack hdqrs jumbo vingt
expwy flock hdqrs jumba vingt
fldxt gconv herbs jimpy quawk
fldxt gconv jerky saqib whump
fldxt gconv jerky squib whamp
...
fldxt nymph squib vejoz wrack
brock japyx seqwl vingt zhmud
glack hdqrs jowpy muntz vibex
brigs fldxt nymph quawk vejoz
ampyx flung hdqrs twick vejoz
fldxt gconv jerky squiz whamp
fldxt grimp quawk synch vejoz
fldxt gryph manqu swick vejoz
fldxt nymph quags vejoz wrick
frock japyx seqwl vingt zhmud
538    2690   16678
```

The CMake project has also defined the `original` target, to build the original code written by @ilyanikolaevsky - this could be useful for validating the results.

# constexpr fun

At a glance, the idea to use the `constexpr` feature of C++ looks brilliant, but there are lots of restrictions of what operations and calculations could be done in compile time, not to mention ENORMOUS compilation time.

Not a long ago only very simple stuff could be executed at compile time - some simple expressions like 2+2 and that would be it. Only recently the C++ standard and compilers allow to do more advanced computations in the compile time. To play with this project I went with compiling the newest available GCC and newest available Clang, as I was trying to use newest parts of C++ standard library available for the constexpr context, e.g. std::bitset, which wasn't available in any "stable" release.
The process of getting this to work was not easy - this feature of C++ is not meant and is not ready for such extensive string processing, at least at this point in time.

Some lesson learned during this experiment:
- can't read any file at the compilation time (there is some proposal which allows embedding data to the application, but it's not yet incorporated into the standard): the only option at this moment is the `#include` directive,
- there is no way in the standard C++ to include an external file's content into a string to be processed later: thus the only dirty trick in this project with CMake, which generates an intermediate input file; all it does is putting in the begining and the end of the file C++'s raw multiline literal markups (`R"(` and `)"`) - in this form it could be safely included into a C++ source file,
- better to avoid using any containers from C++ std::lib: the cost of compilation time and memory footprint is insanely big, compared to the plain arrays,
- the compiler was sometimes complaining about modifying anything passed by a reference to the consteval function,
- you can't return an array from the function, but you can return a struct containing array,
- if you don't use dynamically sized container, you have to use statically sized array (BTW. much better to use plain array than std::array - huge memory savings there),
- considering the above: you must know the size of your results before you actually compute the results; this seems like a paradox, but the solution is painfully simple: you must do your computations twice - first time in order to count how big your results will be, and second time doing the actual job with assigning and returning your results,
- better not to place big constexpr evaluated array inside a regular runtime function, as this will most likely corrupt the runtime stack,
- clang is doing consteval calculations much faster than GCC, but has lower limit of consteval operations' count,
- writing chars to a file is about 2x slower with C++'s `std::ofstream` than with C's `fputc` (at least in my environment).
