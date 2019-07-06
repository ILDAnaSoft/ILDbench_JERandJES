
## ILDbench_JERandJES

Extended JES and JER benchmark

### Installation


- the package dependencies: iLCSoft
- to compile the package:

```shell
source /path/to/ilcsoft/init_ilcsoft.sh
mkdir build
cd build
cmake -C $ILCSOFT/ILCSoft.cmake ..
make install
```

### To run the analysis


```shell
export MARLIN_DLL=./ILDbench_JERandJES/lib/libILDbench_JERandJES.so
Marlin ./ILDbench_JERandJES/xml/PFOAnalysis.xml \
      --constant.lcgeo_DIR=$lcgeo_DIR \
      --constant.DetectorModel=${Detector_Model} \ Detector_Model = ILD_l5_o1_v2/ILD_s5_o1_v2
      --global.LCIOInputFiles=${input_file} \ input_file=input lcio REC/DST file
      --constant.OutputBaseName=${output_file_basename} \ output_file_basename = basename for output root file
```
### Important

Note that after initializing ILCSoft, standard pfoanalysis processor is exported. By exporting libILDbench_JERandJES library, standard pfoanalysis should be removed from library list

### Output:

output root file contains additional branchs compared to the standard pfoanalysis root files that include neutrino corrections for different numbers of semi-leptonic decay. number of semi-lptonic decays of B/C hadron or total (nBSLD/nCSLD/nSLD) are stored in separate branches.

If you want to provide a lot of details on your analysis, use the doc/Readme.md and point to it from this Readme.md file:

More documentation available here in [doc/Readme.md](doc/Readme.md) !

### Issues and contact
- Issues:
    - via the Github issue interface. For the skeleton package: https://github.com/ILDAnaSoft/ILDbench_JERandJES/issues

- contact:
    - email address: yasser.radkhorrami@desy.de
    - working institute: DESY/FLC


