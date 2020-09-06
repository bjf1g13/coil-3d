all: main.c
	 gcc -o COIL-3D main.c -lm `pkg-config --cflags --libs cairo x11`

clean: 
	$(RM) COIL-3D
