# Flow C++ Utilities
By Eugen Sawin <esawin@me73.com>.

## Version
Unstable draft.

## Requirements
* POSIX.1b-compliant operating system (support for librt)
* GNU Make
* GNU GCC 4.7+ (`sudo apt-get install build-essential`)
* Python 2.7+ (optional, for style checking)

## Dependencies
### Required
None.

### Optional
* gtest *for testing* (<http://code.google.com/p/googletest>)
* cpplint *for style checking* (`make cpplint`)

## Building
To build Flow use:

    make

Note that this will just copy the header files into the `include` directory. 

To install Flow use:

    sudo make install

Note that this will copy the header files to the `/usr/include/` directory,
hence requiring *superuser* priviledges.

## Using
### Measure execution time
Include `flow/clock.h` to use the execution time measuring clocks. There
are three classes for clocks: `ThreadClock`, `ProcessClock` and just `Clock`,
measuring *thread CPU-time*, *process CPU-time* and *system wall-clock time*
respectively.

Here is an example for realtime performance measuring:

    using flow::time::Clock;

    Clock begin;  // Stops the begin time.
    // Run the procedures to be measured here.
    cout << Clock() - begin;  // Pretty-prints the duration.

If you want to print the exact duration value without automatic pretty-printing,
just use the `value()` member function:

    using flow::time::Clock;

    Clock begin;  // Stops the begin time.
    // Run the procedures to be measured here.
    auto duration = Clock() - begin;  // Saves the clock difference.
    cout << duration.value();  // Prints the duration in microseconds.

### Pretty-print STL containers 
Include `flow/stringify.h` to use the pretty-print feature for STL containers.
You may use the function `Str` explicitly or just enjoy the overloaded stream
operator.

Here is an example using the overloaded stream operator:

    map<pair<int, string>, vector<int>> nested_map;  // Nasty nested container.
    cout << nested_map;  // Pretty-prints the map;

Here is an example using the `Str` function:

    using flow::io::Str;

    map<pair<int, string>, vector<int>> nested_map;  // Nasty nested container.
    printf("%s", Str(nested_map));  // Pretty-prints the map.

### Serialize STL containers
Include `flow/serialize.h` to serialize and deserialize STL containers. There
are two functions, `Write` and `Read`, to write containers to a stream and
read containers from a stream respectively.

Here is an example to load and save a container to some file:

    using flow::io::Write;
    using flow::io::Read;
  
    map<pair<int, string>, vector<int>> nested_map;  // Nasty nested container.
    fstream some_file("nested-map");  // Some file for the serialization data.
    Write(nested_map, some_file);  // Saves the map to the file.

    map<pair<int, string> vector<int>> nested_map_in;
    Read(some_file, &nested_map_in);  // Loads the map from the file.
    assert(nested_map == nested_map_in);  // Should hold.

## License
The MIT License.
