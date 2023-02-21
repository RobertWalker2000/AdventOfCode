namespace AoC_CS
{
    class Y2015_D14T2
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
            System.Collections.Generic.List<Reindeer> deer = new System.Collections.Generic.List<Reindeer>();

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

                //Create a reindeer with the correct details for use in the race
                deer.Add(new Reindeer(speed, flyTime, restTime));
            }

            const int raceTime = 2503;
            //Track the position of each reindeer every second, awarding a point to the reindeer in the leed
            for (int i = 0; i < raceTime; i++)
            {
                int leadingDist = 0;
                //Update every deer and track the position of the leading deer
                for(int d = 0; d < deer.Count; d++)
                {
                    deer[d].Update();
                    leadingDist = System.Math.Max(deer[d].Distance, leadingDist);
                }

                //Award points to the leading reindeer(s)
                for (int d = 0; d < deer.Count; d++)
                {
                    if (deer[d].Distance == leadingDist)
                        deer[d].AwardPoint();
                }
            }

            int maxPoints = 0;
            foreach (Reindeer d in deer)
                maxPoints = System.Math.Max(maxPoints, d.Points);

            System.Console.WriteLine("Most points: " + maxPoints.ToString());
        }
    }

    class Reindeer
    {
        public Reindeer(int inSpeed, int inFlyTime, int inRestTime)
        {
            speed = inSpeed;
            flyTime = inFlyTime;
            restTime = inRestTime;
        }

        public void Update()
        {
            if (isFlying)
                Fly();
            else
                Rest();
        }

        public void AwardPoint() { Points++; }

        void Fly()
        {
            Distance += speed;

            if(++timeInState >= flyTime)
            {
                isFlying = false;
                timeInState = 0;
            }
        }

        void Rest()
        {
            if(++timeInState >= restTime)
            {
                isFlying = true;
                timeInState = 0;
            }
        }

        readonly int speed, flyTime, restTime;
        bool isFlying = true;
        int timeInState = 0;
        public int Distance { get; private set; } = 0;
        public int Points { get; private set; } = 0;
    }
}