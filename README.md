# C++ XML Generator using pugixml

This project generates **valid XML datasets** using the lightweight [pugixml](https://pugixml.org/) library.

## Features
- Custom XML size, nesting, keys, values, and attributes
- Escaped characters support
- Optional repeated keys, self-closing tags, and attributes

## Setup
1. Download pugixml from [https://pugixml.org/](https://pugixml.org/)
2. Add `pugixml.hpp` and `pugixml.cpp` to this directory
3. Modify the list of valid formats from `main.cpp`

## Build
```bash
g++ main.cpp pugixml.cpp -o xml_generator
```

## Run
```bash
./xml_generator
```

## Output
The program saves a file named `output.xml` in the working directory.
