namespace AoC_CS
{
    class Y2015_D14T1
    {
        public void Solve()
        {
            int day = 14;
            string filePath = @"Inputs\2015\Day " + day.ToString() + ".txt";

            if (!System.IO.File.Exists(filePath))
            {
                System.Console.WriteLine("ERROR: File does not exist");
                return;
            }
            
            string[] input = System.IO.File.ReadAllLines(filePath);
            const int raceTime = 2503;
            int maxDist = 0;

            foreach(string line in input)
            {
                //Use a RegEx to quickly extract all numerical values
                var numbers = System.Text.RegularExpressions.Regex.Matches(line, @"\d+");

                if(numbers.Count != 3)
                {
                    System.Console.WriteLine("ERROR: Unexpected number of matches in RegEx");
                    continue;
                }

                //Read all the numbers into the appropriate variables
                int speed = int.Parse(numbers[0].ToString());
                int flyTime = int.Parse(numbers[1].ToString());
                int restTime = int.Parse(numbers[2].ToString());
                int combinedTime = flyTime + restTime;

                //Find the number of full fly/rest cycles covered in the time and convert it to a distance
                int fullCycles = (int)System.Math.Floor((double)raceTime / (double)combinedTime);
                int distance = fullCycles * speed * flyTime;

                //Now find how much distance was covered in the partial cycle
                int spareTime = raceTime % combinedTime;
                distance += speed * System.Math.Min(spareTime, flyTime);

                maxDist = System.Math.Max(maxDist, distance);
            }

            System.Console.WriteLine("Winning distance: " + maxDist.ToString());
        }
    }
}