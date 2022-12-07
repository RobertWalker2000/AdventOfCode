#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef std::pair<std::string, int> File;
class Directory
{
public:

	std::string name;
	Directory* parent = nullptr;
	int size = 0;
	std::vector<Directory> subDirectory;
	std::vector<File> files;
	Directory* child = nullptr;

	int CalculateSize()
	{
		size = 0;
			
		for (int i = 0; i < files.size(); i++)
			size += files[i].second;

		for (int i = 0; i < subDirectory.size(); i++)
			size += subDirectory[i].CalculateSize();

		return size;
	}

	int FindSmallestValidDir(const int minSize)
	{
		if (size < minSize)
			return INT_MAX;

		int smallestDir = size;

		for (int i = 0; i < subDirectory.size(); i++)
		{
			int temp = subDirectory[i].FindSmallestValidDir(minSize);
			if (temp > minSize && temp < smallestDir)
				smallestDir = temp;
		}

		return smallestDir;
	}

	void AddListing(std::string data)
	{
		int splitPos = data.find(" ");

		if (splitPos < 0 || splitPos > 20)
			std::cout << "Weird splitPos" << std::endl;

		if (data.substr(0, 3) == "dir")	//It's a directory
		{
			Directory tempDir;
			tempDir.name = data.substr(splitPos + 1);
			tempDir.parent = this;

			subDirectory.push_back(tempDir);
		}
		else	//It's a file
		{
			File tempFile;
			tempFile.first = data.substr(splitPos + 1);
			tempFile.second = stoi(data.substr(0, splitPos));

			files.push_back(tempFile);
		}
	}

	Directory* ChangeDirectory(std::string dirName)
	{
		if (dirName == "..")
			return parent;

		for (int i = 0; i < subDirectory.size(); i++)
		{
			if (subDirectory[i].name == dirName)
			{
				child = &subDirectory[i];
			}
		}

		return child;
	}

	bool operator==(Directory other) { return (name == other.name); }
};

int main()
{
	//Create an input stream and open the file
	const int day = 7;
	const int task = 1;

	std::string fileName;
	fileName.append("2022/Inputs/D");
	fileName.append(std::to_string(day));
	fileName.append("T");
	fileName.append(std::to_string(task));
	fileName.append(".txt");
	std::ifstream input;
	input.open(fileName);

	//Check the input stream is valid
	if (!input)
	{
		std::cout << "Unable to open file" << std::endl;
		exit(1);
	}

	Directory topDir;
	topDir.name = "/";

	Directory* workingDir = nullptr;
	while (!input.eof())	//Populate the directories
	{
		//char temp = input.get();
		if (input.get() != '$')	//Ensure we are working with a command
		{
			std::cout << "ERROR: Invalid line start" << std::endl;
			continue;
		}

		input.get();	//Skip the space after the $

		if (input.peek() == 'c')	//Change directory
		{
			input.ignore(256, ' ');	//Skip the command (cd)

			char dirName[20];
			input.getline(dirName, 256);	//Get the new directory name

			if (dirName == std::string("/"))
				workingDir = &topDir;
			else
				workingDir = workingDir->ChangeDirectory(dirName);
		}
		else if (input.peek() == 'l')	//List contents
		{
			input.ignore(256, '\n');	//Skip to the next line

			while (input.peek() != '$' && !input.eof())	//Keep listing files until we reach the next command
			{
				char data[30];
				input.getline(data, 256);
				workingDir->AddListing(data);
			}
		}
	}

	topDir.CalculateSize();
	int freeSpace = 70000000 - topDir.size;
	int spaceNeeded = 30000000 - freeSpace;

	std::cout << "Smallest deletable directory: " << topDir.FindSmallestValidDir(spaceNeeded) << std::endl;

	exit(0);
}
