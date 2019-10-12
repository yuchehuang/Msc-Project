#ifndef A_NIMATION
#   define A_NIMATION

//*****************************************************************************
#include"CImg.h"
#include"CPendulum.h"
#include <gsl/gsl_math.h>
#include <CMatrix/CVector.h>

using namespace cimg_library;

static const int size_x = (int)screen_size_x;
static const int size_y = screen_size_y;
static int size_z = 1;
static const int numberOfColorChannels = 3; // R G B
static const unsigned char initialValue = 0;

// Object color determine 
static const unsigned char color_circle[3] = { 250, 100, 0 };
static const unsigned char white[3] = { 255, 255, 255 };
static const unsigned char color_car[3] = { 255, 255, 0 };


static CImg<unsigned char> blank(size_x, size_y, size_z, numberOfColorChannels, initialValue), paper;
CImgDisplay disp(blank, "Pendulum");


void CImg_plot(CColumnVector& stateVector, uint32_t Episode, int Length_reamin)
	{

	if(disp.is_closed())
		CImgDisplay disp(blank, "Pendulum");

	// initialise the pendulum at the middle of simulated window 
	double car_y = disp.height() / 2;
	double car_x = disp.width() / 2;
	car_x += scale_factor*stateVector[5];

	// Generate the coordinate of each object
	const int m_to_cm = 100;
	double pendulum_1_x = car_x + length * m_to_cm * sin(stateVector[1]);
	double pendulum_1_y = car_y - length * m_to_cm * cos(stateVector[1]);
	double pendulum_2_x = pendulum_1_x + length_2 * m_to_cm * sin(stateVector[2]);
	double pendulum_2_y = pendulum_1_y - length_2 * m_to_cm * cos(stateVector[2]);

	// Plot objects
	paper = blank;
	paper.draw_rectangle(car_x - 10, car_y - 10, car_x + 10, car_y + 10, color_car);
	paper.draw_line(car_x, car_y, pendulum_1_x, pendulum_1_y, white);
	paper.draw_ellipse(pendulum_1_x, pendulum_1_y, 10, 10, 0, color_circle);
	paper.draw_line(pendulum_1_x, pendulum_1_y, pendulum_2_x, pendulum_2_y, white);
	paper.draw_ellipse(pendulum_2_x, pendulum_2_y, 10, 10, 0, color_circle);
	paper.draw_text(0, 0, " Episode: %d\n Length: %d\n Damping constant: %f\n Cart_x: %f \n angule_1: %f \n angule_2: %f \n Length_1: %f \n Length_2: %f \n M1: %f \n M2: %f\n", white, 0, 0.7f, 16, Episode, Length_reamin, 0.1, stateVector[5],stateVector[1], stateVector[2], length, length_2, m1, m2);
	disp.display(paper).wait(5);
	}

#endif // A_NIMATION
