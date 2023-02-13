using System.Collections.Generic;

namespace AoC_CS
{
    class Y2015_D9T1
    {
        public void Solve()
        {
            int day = 9;
            string filePath = @"Inputs\2015\Day " + day.ToString() + ".txt";

            if (!System.IO.File.Exists(filePath))
            {
                System.Console.WriteLine("ERROR: File does not exist");
                return;
            }
            
            string[] input = System.IO.File.ReadAllLines(filePath);
            Dictionary<string, Town> townMap = new Dictionary<string, Town>();

            //Create a dictionary of all the towns
            for(int i = 0; i < input.Length; i++)
            {
                string line = input[i];

                int index = line.IndexOf(" ");
                string startTown = line.Substring(0, index);
                line = line.Remove(0, index + 4);   //Remove the starting town, the word "to" and the attached spaces, leaving us at the start of the next town

                index = line.IndexOf(" ");
                string endTown = line.Substring(0, index);
                line = line.Remove(0, index + 3);   //Remove everything before the distance

                int dist = int.Parse(line);

                //If the town isn't in the dictionary yet, add it
                if (!townMap.ContainsKey(startTown))
                    townMap.Add(startTown, new Town());

                if (!townMap.ContainsKey(endTown))
                    townMap.Add(endTown, new Town());

                //Add a link between towns in both directions
                townMap[startTown].AddLink(endTown, dist);
                townMap[endTown].AddLink(startTown, dist);
            }

            int shortDist = int.MaxValue;
            foreach(string town in townMap.Keys)
            {
                int dist = townMap[town].FindShortRoute(ref townMap);
                shortDist = System.Math.Min(shortDist, dist);
            }

            System.Console.WriteLine("Shortest possible distance: " + shortDist.ToString());
        }
    }

    class Town
    {
        public Town() { }

        Dictionary<string, int> links = new Dictionary<string, int>();
        public bool shouldCheck { get; private set; } = true;

        public void AddLink(string otherTown, int distance)
        {
            links.Add(otherTown, distance);
        }

        public int FindShortRoute(ref Dictionary<string, Town> towns)
        {
            //Start by setting this town to not be checked, to prevent doubling back on ourself
            shouldCheck = false;

            int bestDist = int.MaxValue;
            //Loop through all possible options for our next move, finding the best possible distance from the current node to the end
            foreach(string town in towns.Keys)
            {
                if(towns[town].shouldCheck)
                {
                    int subDist = towns[town].FindShortRoute(ref towns);    //Find the shortest distance when starting with the given node
                    subDist += links[town]; //Add the distance from the current node to the given node for the sub distance
                    bestDist = System.Math.Min(bestDist, subDist);  //Compare the current distance to the best distance and keep the shortest
                }
            }

            //Before we return our route, set shouldCheck back to true so this node can be checked again in other routes
            shouldCheck = true;

            if (bestDist == int.MaxValue)
                return 0;   //If we didn't have any other nodes to check, we have no sub distance to travel
            else
                return bestDist;
        }
    }
}