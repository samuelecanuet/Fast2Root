# Fast2Root

This code is a automatic converter between .fast file to .root file.
## Prerequisites
- **fasterac**
- **ROOT**
- **Signal Dictionnary** (Optionnal)

## Compile

Basic compiling : 
```bash
make
```

SIgnal Dictionnary compiling
```bash
make s
```

Cleaning 
```bash
make clean
```

## Usage
The .setup file and the .fast file must be in the same directory. An automatic iterating loop will read all the following subfast files (_000x). By default the output is saved in the fast file directory, but you can indicate an other path with the option *-o*. If you are using file from multifast mode you have to provide the sample.pid file in the main directory of Fast2Root.

```bash
Fast2Root <path/filename_0001.fast> -o <path_ROOT_Output>
```

## Result
At the end you have a ROOT file in the .fast file directory. 

### Pre-analysed histograms for each channel:
 - Channel
 - Time
 - Saturated *(if supported by the coder)*
 - PileUp *(if supported by the coder)*
 - Channel/Time
 - Channel/Saturated *(if supported by the coder)*
 - Channel/PileUp *(if supported by the coder)*
 - Rate SENT
 - Rate TRIG
 - Rate CALC *(if supported by the coder)*

### Tree
You have 2 TTrees, one for ungrouped data and one for grouped data.

- In normal mode : 
    - *Ungrouped Tree*
        - *(int)* Channel 
        - *(int)* Label
        - *(int)* Time
        - *(int)* PileUp
    - *Grouped Tree*
        - *vector\<int>* Channel
        - *vector\<int>* Label
        - *vector\<int>* Time
        - *vector\<int>* PileUp

- In s mode (Signal ROOT Dictionnary)
    - *Ungrouped Tree*
        - *(Signal)* (Channel, Label, Time, PileUp)
    - *Grouped Tree*
        - *vector\<Signal>* (Channel, Label, Time, PileUp)


## TODO
- Include all the faster data type

## Version
- v2.0 Basic Reader with QDC and CRRC4 implemented
    - v2.1 Adding TRAPEZ
    - v2.2 Support multifast files
    - v2.3 Writing recorded Start/Stop time in the ouput ROOT file
    - v2.4 Saving PileUp tag from Spectro coders
    - v2.5 Saving QDC and SPECTRO counter
        - v2.5.1 Progress counter for subfile reading 
    - v2.6 Excluding data between *MISSING* and *MISSED* flags


## AUTOR
*Samuel Lecanuet lp2iB*