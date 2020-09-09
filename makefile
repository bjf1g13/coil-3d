all: 
	 gcc -o COIL-3D main.c optimiser/config.c viewer/cairo_functions.c optimiser/optimiser.c viewer/viewer.c solver/coil3dMath.c  solver/solver.c -lm -lconfig `pkg-config --cflags --libs cairo x11`

clean: 
	$(RM) COIL-3D
