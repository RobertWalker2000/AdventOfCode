using System.Collections.Generic;

namespace AoC_CS
{
    class D3T2
    {
        public void Solve()
        {
            int day = 3;
            string filePath = @"Inputs\2015\Day " + day.ToString() + ".txt";

            if (!System.IO.File.Exists(filePath))
            {
                System.Console.WriteLine("ERROR: File does not exist");
                return;
            }
            
            string[] input = System.IO.File.ReadAllLines(filePath);

            ICollection<Pair<int, int>> visitedHouses = new HashSet<Pair<int, int>>();
            Pair<int, int> santa = new Pair<int, int>(0, 0);
            Pair<int, int> roboSanta = new Pair<int, int>(0, 0);
            bool isRobo = false;
            visitedHouses.Add(new Pair<int, int>(0, 0));

            while(input[0].Length > 0)
            {
                //Go to the correct santa's location
                Pair<int, int> currentHouse;
                if (isRobo)
                    currentHouse = roboSanta;
                else
                    currentHouse = santa;

                //Get the next instruction and remove it from the list
                char direction = input[0][0];
                input[0] = input[0].Remove(0, 1);

                //Move in the corresponding direction
                switch(direction)
                {
                    case '>':
                        currentHouse.X++;
                        break;
                    case '<':
                        currentHouse.X--;
                        break;
                    case '^':
                        currentHouse.Y++;
                        break;
                    case 'v':
                        currentHouse.Y--;
                        break;
                    default:
                        System.Console.WriteLine("ERROR: Unexpected direction input");
                        return;
                }

                //Add the house to the set if unique
                visitedHouses.Add(new Pair<int, int>(currentHouse.X, currentHouse.Y));

                //Update Santa/Robosanta's position and swap santa
                if (isRobo)
                    roboSanta = currentHouse;
                else
                    santa = currentHouse;

                isRobo = !isRobo;
            }

            //Num elements in set gives num of unique houses with gifts
            int giftedHouses = visitedHouses.Count;
            System.Console.WriteLine("Number of houses with gifts: " + giftedHouses);
        }
    }
}