all: 
	 gcc -o COIL-3D main.c optimiser/optimiser.c viewer/viewer.c viewer/cairo_functions.c solver/solver.c -lm `pkg-config --cflags --libs cairo x11`

clean: 
	$(RM) COIL-3D
