#include <iostream>
#include <fstream>
#include <string>

int main()
{
	//Create an input stream and open the file
	const int day = 3;

	std::string fileName;
	fileName.append("2016/Inputs/D");
	fileName.append(std::to_string(day));
	fileName.append(".txt");
	std::ifstream input;
	input.open(fileName);

	//Check the input stream is valid
	if (!input)
	{
		std::cout << "Unable to open file" << std::endl;
		exit(1);
	}

	int counter = 0;
	while (!input.eof())
	{
		std::string side;
		int a[3];
		int b[3];
		int c[3];

		//First side-----------------------
		//First triangle
		input >> side;
		if (side.length() == 0)
			break;
		a[0] = std::stoi(side);

		//Second triangle
		input >> side;
		a[1] = std::stoi(side);

		//Third triangle
		input >> side;
		a[2] = std::stoi(side);

		//Second side-----------------------
		//First triangle
		input >> side;
		b[0] = std::stoi(side);

		//Second triangle
		input >> side;
		b[1] = std::stoi(side);

		//Third triangle
		input >> side;
		b[2] = std::stoi(side);

		//Third side-------------------------
		//First triangle
		input >> side;
		c[0] = std::stoi(side);

		//Second triangle
		input >> side;
		c[1] = std::stoi(side);

		//Third triangle
		input >> side;
		c[2] = std::stoi(side);


		//Calculations
		for (int i = 0; i < 3; i++)
		{
			bool isValid = true;
			isValid = isValid && ((a[i] + b[i]) > c[i]);
			isValid = isValid && ((a[i] + c[i]) > b[i]);
			isValid = isValid && ((b[i] + c[i]) > a[i]);

			if (isValid)
				counter++;
		}
	}

	std::cout << "Number of valid triangles: " << counter << std::endl;
	exit(0);
}
