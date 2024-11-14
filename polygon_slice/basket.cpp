#include "basket.h"


void Basket::move() {

	x_translation += 0.05 * dir;

	if (x_translation > 0.8)
		dir = -1;
	else if (x_translation < -0.8)
		dir = 1;

}