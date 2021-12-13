#include <iostream>
#include <fstream>

int main()
{
	//Create an input stream and open the file
	std::ifstream inStream;
	inStream.open("Task 1 Input.txt");

	//Check the input stream is valid
	if (!inStream)
	{
		std::cout << "Unable to open file" << std::endl;
		exit(1);
	}

	//If we reach here, file was opened succesfully
	int val1 = 0;
	int val2 = 0;
	int val3 = 0;
	int val4 = 0;

	int sum1 = 0;
	int sum2 = 0;

	int increaseCount = 0;

	//Write the first 3 lines into the first 3 variables
	inStream >> val1;
	inStream >> val2;
	inStream >> val3;

	//Loop through all values in the file, assigning the newest value to val4
	while (inStream >> val4)
	{
		//With a run of 3 values, we can create and compare the 2 sums of 3
		sum1 = val1 + val2 + val3;
		sum2 = val2 + val3 + val4;

		//Compare the 2 sums of 3 and upate the count accordingly
		if (sum2 > sum1)
			increaseCount++;

		//Shift all the values up by 1 so we have the correct first 3 values for the next loop
		val1 = val2;
		val2 = val3;
		val3 = val4;
	}

	//We are done reading from the file, so close it
	inStream.close();

	//Output results, with a verification value to ensure every value was checked
	std::cout << "Number of times the depth increased: " << increaseCount << std::endl;
	std::cout << "Verification: Last value read in and checked was " << val4 << std::endl;

	exit(0);
}
