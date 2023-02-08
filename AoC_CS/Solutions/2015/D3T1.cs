using System.Collections.Generic;

namespace AoC_CS
{
    class D3T1
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
            Pair<int, int> currentHouse = new Pair<int, int>(0, 0);
            visitedHouses.Add(new Pair<int, int>(0, 0));

            while(input[0].Length > 0)
            {
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

                //Push the new position into the list. If it is a duplicate, it will not be added
                visitedHouses.Add(new Pair<int, int>(currentHouse.X, currentHouse.Y));
            }

            //Num elements in set gives num of unique houses with gifts
            int giftedHouses = visitedHouses.Count;
            System.Console.WriteLine("Number of houses with gifts: " + giftedHouses);
        }
    }
}