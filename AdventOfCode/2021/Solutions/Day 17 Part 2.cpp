#include <iostream>

#define XMAX 262
#define XMIN 236
#define YMAX -58
#define YMIN -78
//#define XMAX 30
//#define XMIN 20
//#define YMAX -5
//#define YMIN -10

int CalcMinX();
bool IsInTargetArea(int posX, int posY);

int main()
{
	//Maximum speed the projectile can have when it leaves y=0
	//Speed at this height will be the same up and back down, and projectile will always hit this point
	//Any higher speed value would overshoot the target area
	const int maxYSpeed = abs(YMIN);
	const int maxXSpeed = XMAX;
	const int minYSpeed = YMIN;
	const int minXSpeed = CalcMinX();

	int possibleShots = 0;
	for (int x = minXSpeed; x <= maxXSpeed; x++)
	{
		for (int y = minYSpeed; y <= maxYSpeed; y++)
		{
			int posX = 0;
			int posY = 0;
			int speedX = x;
			int speedY = y;

			while (!IsInTargetArea(posX, posY))
			{
				posX += speedX;
				posY += speedY;

				//Ensure speedX always decreases towards 0, whether it is positive or negative
				if (speedX != 0)
				{
					int xDec = speedX / abs(speedX);
					speedX -= xDec;
				}

				//Always decrease ySpeed by 1
				speedY--;

				//If we overshoot our target, move on
				if (posX > XMAX || posY < YMIN)
					break;
			}

			if (IsInTargetArea(posX, posY))
				possibleShots++;
		}
	}

	std::cout << "Possible shots: " << possibleShots << std::endl;
	return 0;
}

int CalcMinX()
{
	int speed = 0;
	int pos = 0;

	while (pos < XMIN)
	{
		speed++;
		pos += speed;
	}

	return speed;
}

bool IsInTargetArea(int posX, int posY)
{
	if (posX >= XMIN && posX <= XMAX && posY >= YMIN && posY <= YMAX)
		return true;

	return false;
}