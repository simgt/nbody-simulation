#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "nbody.hh"

#define frand() (rand()/(cl_float)RAND_MAX)

int nbody_init(Particle* p, int n) {
	int i;

	cl_float a1,a2,a3,a4,a5,a6,a7,a8;
	cl_float cmr[3];
	cl_float cmrdot[3];

	cl_float alphas = 2.0;
	cl_float body1 = 5.0;
	cl_float bodyn = 1.0;

	cl_float twopi = 6.283185307;

	cl_float alpha1 = alphas - 1.0;
	cl_float fm1 = 1.0/pow(body1,alpha1);
	cl_float fmn = (fm1 - 1.0/pow(bodyn,alpha1))/((cl_float)n - 1.0);
	cl_float zmass = 0.0;
	cl_float coef = 1.0/alpha1;

	for(i=0;i<n;i++) {
		cl_float fmi = fm1 - (cl_float)(i-1)*fmn;
		p[i].position.w() = 1.0/pow(fmi,coef);
		zmass += p[i].position.w();
	}

	cl_float zmbar1 = zmass/(cl_float)n;
	for(i=0;i<n;i++) p[i].position.w() /= zmbar1;
	zmass = (cl_float)n;

	cmr[0] = cmr[1] = cmr[2] = 0.0f;
	cmrdot[0] = cmrdot[1] = cmrdot[2] = 0.0f;

	cl_float ri;
	for(i=0;i<n;i++) {

		cl_float tmp;
		do {
			a1 = frand();
			ri = 1.0/sqrt((pow(a1,(-0.6666667))-1.0));
		} while (ri > 10.0);

		a2 = frand();
		a3 = frand();
		p[i].position.z() = (1.0 - 2.0*a2)*ri;
		p[i].position.x() = sqrt(ri*ri 
			- p[i].position.z()*p[i].position.z())*cos(twopi*a3);
		p[i].position.y() = sqrt(ri*ri 
			- p[i].position.z()*p[i].position.z())*sin(twopi*a3);

		do {
			a4 = frand();
			a5 = frand();
			a6 = a4*a4*pow((1.0-a4*a4),3.5);
		} while (0.1*a5 > a6);

		a8 = a4*sqrt(2.0)/pow((1.0 + ri*ri),0.25);
		a6 = frand();
		a7 = frand();
	
		p[i].velocity.z() = (1.0 - 2.0*a6)*a8;
		p[i].velocity.x() = sqrt(a8*a8 
			- p[i].velocity.z()*p[i].velocity.z())*cos(twopi*a7);
		p[i].velocity.y() = sqrt(a8*a8 
			- p[i].velocity.z()*p[i].velocity.z())*sin(twopi*a7);
		p[i].velocity.w() = 0.0f;

		cmr[0] += p[i].position.w() * p[i].position.x();
		cmr[1] += p[i].position.w() * p[i].position.y();
		cmr[2] += p[i].position.w() * p[i].position.z();
		cmrdot[0] += p[i].position.w() * p[i].position.x();
		cmrdot[1] += p[i].position.w() * p[i].position.y();
		cmrdot[2] += p[i].position.w() * p[i].position.z();

	}

//printf("%f %f %f %f %f %f\n",cmr[0],cmr[1],cmr[2],cmrdot[0],cmrdot[1],cmrdot[2]);

	a1 = 1.5*twopi/16.0;
	a2 = sqrt(zmass/a1);
	for(i=0;i<n;i++) {

		p[i].position.x() -= cmr[0]/zmass;
		p[i].position.y() -= cmr[1]/zmass;
		p[i].position.z() -= cmr[2]/zmass;
		p[i].velocity.x() -= cmrdot[0]/zmass;
		p[i].velocity.y() -= cmrdot[1]/zmass;
		p[i].velocity.z() -= cmrdot[2]/zmass;
		p[i].position.x() *= a1;
		p[i].position.y() *= a1;
		p[i].position.z() *= a1;
		p[i].velocity.x() *= a2;
		p[i].velocity.y() *= a2;
		p[i].velocity.z() *= a2;
	}

//	for(i=0;i<n;i++) {
//		printf("%d %e %e %e %e %e %e\n",
//			i,pp[4*i+0],pp[4*i+1],pp[4*i+2],vv[4*i+0],vv[4*i+1],vv[4*i+2]);
//	}

	cl_float ppmin = p[0].position.x();	
	cl_float ppmax = p[0].position.x();	
	for(i=0;i<n;i++) ppmin = (ppmin<p[i].position.x())? ppmin:p[i].position.x();
	for(i=0;i<n;i++) ppmin = (ppmin<p[i].position.y())? ppmin:p[i].position.y();
	for(i=0;i<n;i++) ppmin = (ppmin<p[i].position.z())? ppmin:p[i].position.z();
	for(i=0;i<n;i++) ppmax = (ppmax>p[i].position.x())? ppmax:p[i].position.x();
	for(i=0;i<n;i++) ppmax = (ppmax>p[i].position.y())? ppmax:p[i].position.y();
	for(i=0;i<n;i++) ppmax = (ppmax>p[i].position.z())? ppmax:p[i].position.z();

}
