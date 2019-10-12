#include "CPendulum.h"
#include<stdio.h>



//double Force_A = 0, Cart_force = 0, Sv = 0, Sx = (disp.width() / 2), Sy = (disp.height() / 2);
//double k = 0.1, a = 0, b = 0;

//*****************************************************************************

CPendulum::CPendulum(CColumnVector& stateVector)
// Constructor
	{
	stateVector[1] = theta_1;
	stateVector[2] = theta_2;
	stateVector[3] = theta_1_velocity;
	stateVector[4] = theta_2_velocity;
	stateVector[5] = cart_x;
	cart_velocity = 0;
	t = 0.0;
	}

//-----------------------------------------------------------------------------

CPendulum::~CPendulum()
	{
	//Empty
	}

//-----------------------------------------------------------------------------

bool CPendulum::UpdateStateVector(double cartForce, CColumnVector& stateVector)
	{
	const double Time_step = 0.01;
	gsl_odeiv2_system pendulum_sys = {func, NULL, 6, &cartForce};
	gsl_odeiv2_driver* pendulum_driver_system = gsl_odeiv2_driver_alloc_y_new(&pendulum_sys, gsl_odeiv2_step_rk4, 1e-6, 1e-6, 0.0);

	//------------derivation processing----------------------------//
	double status[6] = {stateVector[1], stateVector[2], stateVector[3], stateVector[4], stateVector[5], cart_velocity};
	int process_chek_flag = gsl_odeiv2_driver_apply(pendulum_driver_system, &t, t + Time_step, status);
	if(process_chek_flag != GSL_SUCCESS)
		{
		printf("error, cart force =%lf\n", cartForce);
		return false;
		}

	//----represent theta between -3.14 to 3.14--------------------//
	while(status[0] > M_PI)
		status[0] -= (2 * M_PI);
	while(status[0] <= -M_PI)
		status[0] += (2 * M_PI);

	while(status[1] > M_PI)
		status[1] -= (2 * M_PI);
	while(status[1] <= -M_PI)
		status[1] += (2 * M_PI);
	//-------------------------------------------------------------//

	stateVector[1] = status[0];
	stateVector[2] = status[1];
	stateVector[3] = status[2];
	stateVector[4] = status[3];
	stateVector[5] = status[4];
	cart_velocity = status[5];


	// set as cyclic track
    while(stateVector[5] > (screen_size_x/(2*scale_factor)))    //animation size/2
		stateVector[5]-= (screen_size_x/scale_factor);
	while(stateVector[5] < -(screen_size_x/(2*scale_factor)))
		stateVector[5] += (screen_size_x/scale_factor);


	gsl_odeiv2_driver_free(pendulum_driver_system);
	return true;
	} // CPendulum::UpdateStateVector()


//-----------------------------------------------------------------------------

int  func(double t, const double y[], double f[], void* params)
	{
	/*

	y[0]    = theta_1
	y[1]    = theta_2
	y[2]    = velocity_1
	y[3]    = velocity_2
	y[4]    = cart_x
	y[5]    = cart_velocity

	f[0]    = velocity_1
	f[1]    = velocity_2
	f[2]    = acceleration_1
	f[3]    = acceleration_2
	f[4]    = cart_velocity
	f[5]    = cart_acceleration
	*/

	(void)(t); /* avoid unused parameter warning */
	double th_1   = y[0];
	double th_2   = y[1];
	double vol_1  = y[2];
	double vol_2  = y[3];
	double Cart_velocity =  y[5];
	double cartForce = *(double*)params;
	const double damping_factor = 0.1;
	const double kinetic_friction_factor = 0.01;
	const double Max_static_force = 0.1;
	double damping_element_1 = damping_factor * vol_1;
	double damping_element_2 = damping_factor * vol_2;

	f[0] = vol_1;
	f[1] = vol_2;
	f[2] = (-cartForce/length) * cos(th_1 ) + (((m2 * length * vol_1 * vol_1 * sin(2 * (th_1 - th_2))) + (2 * m2 * length_2 * vol_2 * vol_2 * sin(th_1 - th_2)) + (2 * g * m2 * cos(th_2) * sin(th_1 - th_2)) + (2 * g * m1 * sin(th_1)) + (2 * damping_element_1 - 2 * damping_element_2 * cos(th_1 - th_2))) / (-2 * length * (m1 + m2 * sin(th_1 - th_2) * sin(th_1 - th_2))));
	f[3] = (-cartForce/length_2) * cos(th_2) + (((m2 * length_2 * vol_2 * vol_2 * sin(2 * (th_1 - th_2))) + (2 * (m1 + m2) * length * vol_1 * vol_1 * sin(th_1 - th_2)) + (2 * g * (m1 + m2) * cos(th_1) * sin(th_1 - th_2)) + (2 * damping_element_1 * cos(th_1 - th_2) - ((2 * (m1 + m2) * damping_element_2) / m2))) / (2 * length_2 * (m1 + m2 * sin(th_1 - th_2) * sin(th_1 - th_2))));
	f[4] = Cart_velocity;
	f[5] = cartForce/(m1+m2+Cart_mass);

	return GSL_SUCCESS;
	}


//*****************************************************************************





