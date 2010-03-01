#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
     
     int
     main (void)
     {
          const gsl_rng_type **t, **t0;
          
          t0 = gsl_rng_types_setup ();
          
          printf ("Available generators:\n");
          
          for (t = t0; *t != 0; t++)
            {
              printf ("%s\n", (*t)->name);
            }

       const gsl_rng_type * T;
       gsl_rng * r;
     
       int i, n = 10;
       double mu = 3.0;
     
       /* create a generator chosen by the 
          environment variable GSL_RNG_TYPE */
     
       gsl_rng_env_setup();
     
       T = gsl_rng_default;
       r = gsl_rng_alloc (T);
     
       /* print n random variates chosen from 
          the poisson distribution with mean 
          parameter mu */
     
       for (i = 0; i < n; i++) 
         {
           unsigned int k = gsl_ran_poisson (r, 0.2);
           printf (" %u", k);
         }
     
       printf ("\n");
       gsl_rng_free (r);
       return 0;
     }
