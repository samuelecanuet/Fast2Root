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
The .setup file and the .fast file must be in the same directory. An automatic iterating loop will read all the following subfast files (_000x).

```bash
Fast2Root <path/filename_0001.fast> 
```

## Result
At the end you have a ROOT file in the .fast file directory. 

### Pre-analysed histograms for each channel:
 - Channel
 - Time
 - Channel/Time

### Tree
You have 2 TTrees, one for ungrouped data and one for grouped data.

- In normal mode : 
    - *Ungrouped Tree*
        - *(int)* Channel 
        - *(int)* Label
        - *(int)* Time
    - *Grouped Tree*
        - *vector\<int>* Channel
        - *vector\<int>* Label
        - *vector\<int>* Time

- In s mode (Signal ROOT Dictionnary)
    - *Ungrouped Tree*
        - *(Signal)* (Channel, Label, Time)
    - *Grouped Tree*
        - *vector\<Signal>* (Channel, Label, Time)


## TODO
- Include all the faster data type

## Version
- v2.0 Basic Reader with QDC and CRRC4 implemented
- v2.1 Adding TRAPEZ
- v2.2 Support multifast files

## AUTOR
*Samuel Lecanuet lp2iB*