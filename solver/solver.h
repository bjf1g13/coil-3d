#ifndef SOLVER_H
#define SOLVER_H

// Calculate the self inductance of the inductor
double self_inductance(double d_o, double d_i, int n, double mu);
// Calculate the mutual inductance of the inductor
double mutual_inductance(double d_o, double d, double mu, double w, double s, int n);
// Calculate the resistance of the indcutor
double resistance(double f, double l, double w);
// Calulate the capacitance of the inductor
double capacitance(double l, double s);
// Calculate the efficiency of a given inductor
double efficiency(double m, double l, double r, double c, double f);
// Run the solver
double solve(double d_o, double w, double s, int n, double mu, double d, double f, double RL, cairo_t *ctx);
#endif // SOLVER_H
