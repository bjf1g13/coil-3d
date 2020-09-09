# COIL-3D : A CAD Tool for the Optimisation of Inductive Links in 3D-ICs
COIL-3D is a CAD tool for determining best performing inductor geometries for use in Inductive Coupling Link (ICL) based Three Dimensional Integrated Circuits (3D-ICs), developed at the University of Southampton, in collaboration with Arm Research.

The tool takes as its inputs a configuration file (*.cfg) describing the physical and electrical parameters required for the design and then generates an inductor layout to maximise the performance of the inductive channel, for data transmission at a given frequency.

COIL-3D consists of two main elements: 
* The solver: a set of mathematical expressions to compute the electrical characteristics of the link directly from its physical parameters (see below),

![COIL-3D Concept](https://github.com/bjf1g13/coil-3d/blob/master/docs/solver.png)

* The optimiser: the optimisation algorithm used to determine best-performing layouts

The tool also provides a gui viewer utility, to visualise the otpimization process and allows export of:
1. A SPICE netlist of the link
2. A DXF file containing the physical inductor geometry
3. A HFSS project file containing the generated inductor 

![COIL-3D GUI](https://github.com/bjf1g13/coil-3d/blob/master/docs/viewer.png)

Variants of COIL-3D have been presented at Design, Automation and Test in Europe Conference and Exhibition (DATE) 2018, and more recently in IEEE Transactions on Very Large Scale Integration (VLSI) Systems, 27 (3).
If using COIL-3D for your research, please cite this paper:

> Fletcher, Benjamin J., Das, Shidhartha and Mak, Terrence (2019) Design and optimization of inductive-coupling links for 3-D-ICs. IEEE Transactions on Very Large Scale Integration (VLSI) Systems, 27 (3), 711-723. (doi:10.1109/TVLSI.2018.2881075).

A pdf version of this paper can also be [downloaded here](https://eprints.soton.ac.uk/428192/1/08561260.pdf).

## Usage
### Dependacnies 
The COIL-3D commandline parser uses the libconfig libarary and the COIL-3D viewer uses the 
sudo apt-get install libconfig-dev
apt-get install libcairo2-dev

### Compiling COIL-3D
The COIL-3D tool can be compiled using the makefile provided, by cloning the repositry and running make:
```
git clone https://github.com/bjf1g13/coil-3d.git
cd coil-3d; make
```
By default the COIL-3D tool also compiles a lightwight viewer utility to graphically show the optimisation process, which relies on X11.lib

![COIL-3D Viewer](https://github.com/bjf1g13/coil-3d/blob/master/docs/viewer.png)

### Running COIL-3D

## File Structure

Directory structure:
Directory | Description
--------- | -----------
./solver | Expressions for calculating the **electrical** parameters of a given layout from its **physical** layout parameters
./optimiser | The main optimisation algorithm used for determining best-performing coil layouts
./viewer | The COIL-3D graphical viewer


