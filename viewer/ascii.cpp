void print_performance (double l, double m double r, double c, double q, double eta) {
  char L_message[50];
  char M_message[50];
  char k_message[50];
  char R_message[50];
  char C_message[50];
  char Q_message[50];
  char eta_message[50];
  
  sprintf(L_message, "Self inductance is : %.10e H",L);
  sprintf(M_message, "Mutual inductance is : %.10e H",M);
  sprintf(k_message, "Coupling Coefficient : %f",k);
  sprintf(R_message, "Rs : %f Ohms",R);
  sprintf(C_message, "C : %.10e F",C);
  sprintf(Q_message, "Q-Factor : %f",Q);
  sprintf(eta_message, "Efficiency : %f %%",eta*100);
}
