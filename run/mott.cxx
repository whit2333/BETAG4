{

//gRoot->Reset();
Double_t res;



TF2 * sigma = new TF2("mott scattering", "(1/x)*(1/x)*(1/x)*(1/x)*(5.77087*(0.261121+x*x*cos(y/2)*cos(y/2)))*(1/sin(x))*(1/sin(x))*(1/sin(x))*(1/sin(x))" ,              0.5,3. ,         0.1 , 3.14159265/2     );

sigma->Draw();

}
