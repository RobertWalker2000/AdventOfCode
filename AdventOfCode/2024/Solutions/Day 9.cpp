#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

uint64_t CalculateDefragmentedChecksum(std::ifstream* input);
uint64_t CalculateBlockDefragmentedChecksum(std::ifstream* input);
uint64_t CalculateChecksumOfStreak(int ID, int count, int pos);

int main()
{
	//Create an input stream and open the file
	const int day = 9;

	std::string fileName;
	fileName.append("2024/Inputs/D");
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

	uint64_t checksum = CalculateDefragmentedChecksum(&input);

	input.seekg(input.beg);
	uint64_t smartChecksum = CalculateBlockDefragmentedChecksum(&input);

	std::cout << "Checksum of defragmented data: " << checksum << std::endl;
	std::cout << "Checksum of block defragmented data: " << smartChecksum << std::endl;

	exit(0);
}

//WARNING: Data must be formatted using UNIX (LF) line breaks. CR LF will cause theend of the file to be read incorrectly
uint64_t CalculateDefragmentedChecksum(std::ifstream* input)
{
	uint64_t checksum = 0;

	input->seekg(input->beg, input->end);	//Move the cursor to the end of the file
	int len = input->tellg();	//Get the length of the file

	//If the input is even, then the last chunk is empty space and we can remove it
	if (len % 2 == 0)
		len--;

	//Represents the ID of the data chunks as read in left to right
	int forID = 0;

	//Calculate the ID that will be given to the last chunk of data
	int backID = ((len - 1) / 2);

	//Declare a forward cursor that will start at the beginning of the file
	int forCursor = input->beg;

	//Move the reverse cursor to just before the last char in the line. This allows us to peek at said char to read its value
	int backCursor = len - 1;

	//Create a variable to track the position in the fully expanded form, which we need to calculate the checksum
	int pos = 0;

	//Prepare the first chunk of data to be read in from the back
	std::pair<int, int> backDataChunk(backID, 0);
	input->seekg(backCursor);
	backDataChunk.second = (input->peek() - '0');

	//Loop through all the data chunks and gaps, working from both ends
	while (backCursor > forCursor)
	{
		//Read in the next data chunk that is not being moved
		input->seekg(forCursor);
		int sizeOfChunk = (input->peek() - '0');

		//Calculate the checksum for this chunk and add it to the total
		checksum += CalculateChecksumOfStreak(forID, sizeOfChunk, pos);
		pos += sizeOfChunk;
		forID++;

		//Move to the empty space and get its length
		forCursor++;
		input->seekg(forCursor);

		int sizeOfGap = (input->peek() - '0');

		forCursor++;

		while (sizeOfGap > 0 && backCursor > forCursor)
		{
			//If the gap can only fit part of the data chunk in, move what we can then move on
			if (sizeOfGap < backDataChunk.second)
			{
				//Move the data to the gap
				checksum += CalculateChecksumOfStreak(backDataChunk.first, sizeOfGap, pos);

				//Reduce the size of the leftover data and update the pos for the checksum
				backDataChunk.second -= sizeOfGap;
				pos += sizeOfGap;
				sizeOfGap = 0;

				break;
			}

			//We can fit the whole chunk in the gap. So move the full chunk
			checksum += CalculateChecksumOfStreak(backDataChunk.first, backDataChunk.second, pos);
			pos += backDataChunk.second;

			//Update the size of the gap
			sizeOfGap -= backDataChunk.second;

			//Update the ID of the data chunk
			backDataChunk.first--;

			//Move backwards to the next data chunk
			backCursor -= 2;

			//Read in the next chunk reeady for the next loop
			input->seekg(backCursor);
			backDataChunk.second = (input->peek() - '0');
		}

	}

	//If we have any leftover data chunk at the back, process it now
	checksum += CalculateChecksumOfStreak(backDataChunk.first, backDataChunk.second, pos);

	return checksum;
}

//Assumes there are no *data* chunks with size 0
uint64_t CalculateBlockDefragmentedChecksum(std::ifstream* input)
{
	using DataChunk = std::pair<int, int>;
	std::multimap<int, int> gaps;	//Map of <size, pos>
	std::vector<std::pair<DataChunk, int>>  dataChunks;	//Vector of <DataChunk(ID, size), pos>

	int ID = 0;
	int pos = 0;
	while (!input->eof())
	{
		//read in the data chunk
		char nextChar = input->get();
		if (nextChar == '\n')
			break;

		//Store the chunk in the vector
		int chunkLen = nextChar - '0';
		DataChunk chunk(ID, chunkLen);
		dataChunks.push_back(std::pair<DataChunk, int>(chunk, pos));
		pos += chunkLen;
		ID++;

		//Read in the gap
		nextChar = input->get();
		if (nextChar == '\n')
			break;

		//Store the gap in the map
		chunkLen = nextChar - '0';

		if (chunkLen == 0)	//Don't need to process gaps of size 0
			continue;

		gaps.insert(std::pair<int, int>(chunkLen, pos));
		pos += chunkLen;
	}

	//Now that we know the size and positions (and IDs) of all the gaps and data chunks, start working backwards to try ad fit the data chunks into the gaps
	for (int i = dataChunks.size() - 1; i >= 0; i--)
	{
		//Get the length of the data chunk
		int chunkLen = dataChunks[i].first.second;

		//Get the smallest available gap that can fit the chunk
		auto gapToCheck = gaps.lower_bound(chunkLen);

		//If there was no gap, leave the chunk alone and go to the next one
		if (gapToCheck == gaps.end())
			continue;

		//Find the earliest possible gap that can fit the data chunk
		auto gapToFill = gapToCheck;
		while (gapToCheck != gaps.end())
		{
			if (gapToCheck->second < gapToFill->second)
				gapToFill = gapToCheck;

			gapToCheck++;
		}

		//Ensure the gap occurs *before* the data chunk, otherwise it doesn't move
		if (gapToFill->second > dataChunks[i].second)
			continue;

		//We found an appropriate gap, move the chunk into it
		dataChunks[i].second = gapToFill->second;

		//Make a gap representing the leftover gap after inserting the data chunk
		std::pair<int, int> newGap = *gapToFill;
		newGap.first -= chunkLen;
		newGap.second += chunkLen;

		//Delete the old gap from the map
		gaps.erase(gapToFill);

		//If the new gap is larger than 0, add it to the map
		if (newGap.first > 0)
			gaps.insert(newGap);
	}

	//Now that we have rearranged all the data chunks, we should calculate the checksum
	uint64_t checksum = 0;
	for (int i = 0; i < dataChunks.size(); i++)
		checksum += CalculateChecksumOfStreak(dataChunks[i].first.first, dataChunks[i].first.second, dataChunks[i].second);

	return checksum;
}

uint64_t CalculateChecksumOfStreak(int ID, int count, int pos)
{
	uint64_t result = 0;

	for (int i = 0; i < count; i++)
	{
		result += (ID * pos);
		pos++;
	}

	return result;
}