# COIL-3D : A CAD Tool for the Optimisation of Inductive Links in 3D-ICs

COIL-3D is a CAD tool for determining best performing inductor geometries for use in Inductive Coupling Link (ICL) based Three Dimensional Integrated Cirucits, developed at The University of Southampton, in collaboration with Arm Research.

The tool takes as its inputs a technology file (\*.tf) describing the thickenesses of each layer within the 3D stack and a optimiser configuration file (\*.cfg) to specify the electrical and physical optimisation constraints. The tool then generates an inductor layout to maxmise the performance of the inductive channel, at a given frequency.

## Usage
The COIL-3D tool can be compiled using the makefile provided, by cloning the repositry and running make:
```
git clone https://github.com/bjf1g13/coil-3d.git
cd coil-3d; make
```
By default the COIL-3D tool also compiles a lightwight viewer utility to graphically show the optimisation process, which relies on X11.lib

## References

## Dependacnies 
The COIL-3D commandline parser uses the libconfig libarary and the COIL-3D viewer uses the 
sudo apt-get install libconfig-dev
apt-get install libcairo2-dev

## File Structure

Directory structure:
Directory | Description
--------- | -----------
./solver | Expressions for calculating the **electrical** parameters of a given layout from its **physical** layout parameters
./optimiser | The main optimisation algorithm used for determining best-performing coil layouts
./viewer | The COIL-3D graphical viewer


