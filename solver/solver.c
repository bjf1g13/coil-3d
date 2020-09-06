// Calculate the self inductance of the inductor
double self_inductance(double d_o, double d_i, int n, double mu)
{
   double rho =  (d_o - d_i)/(d_o + d_i); // Calculate Fill Factor
   double davg = (d_o + d_i)/2;           // Calculate d_avg
    
    return ((1.27 * mu * pow(n,2) * davg)/2) * (log( 2.07/rho ) + (0.18 * rho) + (0.13 * pow(rho,2)));
}

// Calculate the mutual inductance of the inductor
double mutual_inductance(double d_o, double d, double mu, double w, double s, int n)
{
   double ai = d_o / 2; // ri is the radius of the ith turn
   double bi = d_o / 2; // ri is the radius of the ith turn
   
   double Mtot = 0.0;
   double alphai = 0.0;
   int i,j;

   for (i=0;i<n;i++)
   {
      for(j=0;j<n;j++)
      {
         alphai = 2 * (sqrt( (ai * bi) / (pow((ai + bi),2) + pow(d,2)))); // alpha of the ith turn
          
         double Ki = Complete_Elliptic_Integral_First_Kind('k',alphai);
         double Ei = Complete_Elliptic_Integral_Second_Kind('k',alphai);

         double ri = sqrt(pow((ai - bi),2) + pow(d,2));
         double kf = ri/d;
         
         Mtot = Mtot + ((1/(pow(kf,5))) * (((2 * mu)/alphai) * sqrt(ai * ai)) * ( (1 - ((pow(alphai,2))/2))*Ki - Ei) );
         
         ai = ai - (w+s);

      }

      ai = d_o / 2; // Reset First Coil radius
      bi = bi - (w+s);
   } 
   return Mtot;
}

// Calculate the resistance of the indcutor
double resistance(double f, double l, double w) // Does not include proximity Effect
{
   double Rs = sqrt((3.141562 * f * 1.256E-6)/(58.5E6));
   return (l * Rs)/(4E-6 + (2*w));
}

// Calulate the capacitance of the inductor
double capacitance(double l, double s)
{
   return (8.854E-12 * l * 2E-6)/s;
}

// Calculate the efficiency of a given inductor
double efficiency(double m, double l, double r, double c, double f) {

  // Coupling coefficient K = M/L (if both coils are the same)
  double k = M/L;
  
  // Caclulate quality factor
  double Q = (2*M_PI*f*L)/R;
  double QL = 1/((R * sqrt(C/L))+(1/RL * sqrt(L/C)));
  double eta = ((pow(k,2)*Q*QL)/(1+pow(k,2)*Q*QL))*(QL/(Q + QL));

  double Ae = (R * pow(L,2) + R * pow(M,2));
  double Be = (R * pow(RL,2) * (pow(C,2) * pow(R,2) - 2 * C * L) + pow(L,2) * R);
  double Ce = R + RL;
  double omega = 2 * M_PI * f;

  return (pow(omega,2) * pow(M,2) * RL)/((Ae * pow(RL, 2) * pow(C, 2))*pow(omega,2) + (Be + Ce * pow(M, 2))*pow(omega,2) + pow(Ce,2)*RL);
}

// Run the solver
double solve(double d_o, double w, double s, int n, double mu, double d, double f, double RL )
{
      double tab = d_o /50;
      double d_i = 0.00;

      // If the track spacing is not provided, figure it out from the other parameters
      if(s == 0) // In the case that s is automatic
      {
        s = (((d_o - d_i)/2) - (n*w))/(n-1);
        if (s < 0)
        {
           printf("Impossible track width selected");
        }    
      }
      
      // If the inner diameter is not provided, figure it out from the other parameters
      else if(d_i == 0) 
      {
         d_i = d_o - ((2*n)*w + (2*(n-1))*s);
         if (d_i < 0)
         {
            printf("Impossible track width selected");
         }
      }

//      double l = draw_inductor(ctx, n, d_o/SCALE, w/SCALE, s/SCALE, tab/SCALE);

      /* Print Messages */
      double M = mutual_inductance(d_o, d, mu, w, s, n);
      double L = self_inductance(d_o, d_i, n, mu);
      double R = resistance(f, l, w);
      double C = capacitance(l,s);
      double eta = efficiency(M, L, R, C, f) {


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

/*
      cairo_set_font_size(ctx,13);
      
      cairo_set_source_rgb (ctx, 1, 1, 1);
      cairo_move_to(ctx, 20, 20);
      cairo_show_text(ctx, L_message);
      cairo_move_to(ctx, 20, 40);
      cairo_show_text(ctx, M_message);
      cairo_move_to(ctx, 20, 60);
      cairo_show_text(ctx, k_message);

      cairo_set_font_size(ctx,11);
      cairo_move_to(ctx, WIDTH -150, HEIGHT-20);
      cairo_show_text(ctx, R_message);
      cairo_move_to(ctx, WIDTH -150, HEIGHT-40);
      cairo_show_text(ctx, C_message);
      cairo_move_to(ctx, WIDTH -150, HEIGHT-60);
      cairo_show_text(ctx, Q_message);

      cairo_set_source_rgb (ctx, 0.2, 1, 0.2);
      cairo_set_font_size(ctx,13);
      cairo_move_to(ctx, 20, HEIGHT-20);
      cairo_show_text(ctx, eta_message);
*/
      return eta;
}
