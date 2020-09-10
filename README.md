# COIL-3D : A CAD Tool for the Optimisation of Inductive Links in 3D-ICs
COIL-3D is a CAD tool for determining best performing inductor geometries for use in Inductive Coupling Link (ICL) based Three Dimensional Integrated Circuits (3D-ICs), developed at the University of Southampton, in collaboration with Arm Research.

The tool takes as its inputs a configuration file (*.cfg) describing the physical and electrical parameters required for the design and then generates an inductor layout to maximise the performance of the inductive channel, for data transmission at a given frequency.

COIL-3D consists of two main elements: 
* The solver: a set of mathematical expressions to compute the electrical characteristics of the link directly from its physical parameters (see below),

<img src="https://github.com/bjf1g13/coil-3d/blob/master/docs/solver_image.png" width="100%">

* The optimiser: the optimisation algorithm used to determine best-performing layouts

The tool also provides a gui viewer utility, to visualise the otpimization process and allows export of:
1. A SPICE netlist of the link
2. A DXF file containing the physical inductor geometry
3. A HFSS project file containing the generated inductor 

<p align="center"><img src="https://github.com/bjf1g13/coil-3d/blob/master/docs/viewer.png" width="400px"></p>

Variants of COIL-3D have been presented at Design, Automation and Test in Europe Conference and Exhibition (DATE) 2018, and more recently in IEEE Transactions on Very Large Scale Integration (VLSI) Systems, 27 (3).
If using COIL-3D for your research, please cite this paper:

> Fletcher, Benjamin J., Das, Shidhartha and Mak, Terrence (2019) Design and optimization of inductive-coupling links for 3-D-ICs. IEEE Transactions on Very Large Scale Integration (VLSI) Systems, 27 (3), 711-723. (doi:10.1109/TVLSI.2018.2881075).

A pdf version of this paper can also be [downloaded here](https://eprints.soton.ac.uk/428192/1/08561260.pdf).

## Usage

### Dependancies
Compiling COIL-3D tool requires libconfig and cairo to be installed and avaialable on your system. Using the aptitude package manager, these can be installed using:
```bash
> apt-get install libconfig-dev
> apt-get install libcairo2-dev
```

### Compiling COIL-3D
Once the required libraries are available, the COIL-3D tool can be compiled using the makefile provided, by cloning the repositry and running make:
```bash
> git clone https://github.com/bjf1g13/coil-3d.git
> cd coil-3d
> make
```

### Running COIL-3D
The COIL-3D tool is, by default, supplied with an example configuration file to specify the maximum inner and outer area of the inductor, the min/max matal width/spacing, the communication distance, the target operating frequency etc.
This configuration file can be edited as required, and passed to the solver using the '-c' flag:

```bash
~/coil-3d$ ./COIL-3D -c example_config.cfg -t example_techfile.xml -o inductor_1
```
The tool also takes an HFSS technology file as an argument (using the '-t') flag, to specify the layer thicknesses within the 3D stack. Finally, an optional argument -o can also be used to specify the name of the output directory where the generated output data/design will be saved.

An example runme script (./RUNME_EXAMPLE.sh) is provided to run a demo of the tool with these default example templates, but each of the parameters in the technology and configuration files can be adjusted according to the specific project you are working on.

The arguments available to the COIL-3D tool can also be displayed using the help flag:
```bash
~/coil-3d$ ./COIL-3D -h
COIL-3D [-v] [-c configuration inputfile] [-t technology inputfile] [-o output dir]  [-h]
```
## File Structure

Directory structure:
Directory | Description
--------- | -----------
./docs | Images and documentation related to the tool
./solver | Expressions for calculating the **electrical** parameters of a given layout from its **physical** layout parameters
./optimiser | The main optimisation algorithm used for determining best-performing coil layouts
./viewer | The COIL-3D graphical viewer


