#include <iostream>

#define XMAX 262
#define XMIN 236
#define YMAX -58
#define YMIN -78
//#define YMAX -5
//#define YMIN -10

int main()
{
	//Maximum speed the projectile can have when it leaves y=0
	//Speed at this height will be the same up and back down, and projectile will always hit this point
	//Any higher speed value would overshoot the target area
	const int maxSpeed = abs(YMIN);


	int speed = maxSpeed;
	int yPos = YMIN;

	while (speed != 0)
	{
		yPos += speed;
		speed--;
	}
	std::cout << "Maximum height: " << yPos << std::endl;
	return 0;
}