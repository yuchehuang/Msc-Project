#ifndef C_PENDULUM
#   define C_PENDULUM

//*****************************************************************************

#include <CMatrix/CVector.h>
#include <gsl/gsl_odeiv2.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>
#include <Generic/ErrorHandler/ErrorHandler.h>


const double trackLength =12.50; // 2m  track Length
const double screen_size_x=1250.0;
const int screen_size_y=500;
const double scale_factor= screen_size_x/trackLength;
const double rad = M_PI / 180;
const double g = -9.8;		//gravity
const double length = 1.0;  // 100 cm
const double length_2 = 1.0;// 100 cm
const double  m1 = 1.0;
const double  m2 = 1.0;
const double  Cart_mass =1.0;



class CPendulum
	{
	public:
		CPendulum(CColumnVector& stateVector);
		~CPendulum();

		bool UpdateStateVector(double cartForce, CColumnVector& stateVector);
		double cart_velocity = 0;

	private:
		const double theta_1 = 180 * rad;
		const double theta_1_velocity = 0;

		const double theta_2 = 180 * rad;
		const double theta_2_velocity = 0;
		const double cart_x = 0;
		double t = 0.0; //strated time
	};

//*****************************************************************************
int func(double t, const double y[], double f[], void* params);



#endif // C_PENDULUM
