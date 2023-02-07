namespace AoC_CS
{
    class D1T1
    {
        public void Solve()
        {
            int day = 1;
            string filePath = @"Inputs\2015\Day " + day.ToString() + ".txt";

            if(!System.IO.File.Exists(filePath))
            {
                System.Console.WriteLine("ERROR: File does not exist");
                return;
            }
            
            string[] input = System.IO.File.ReadAllLines(filePath);
            string line = input[0];

            int floor = 0;
            while(line.Length > 0)
            {
                char instruction = line[0];

                if (line[0] == '(')
                    floor++;
                else if (line[0] == ')')
                    floor--;
                else
                {
                    System.Console.WriteLine("ERROR: Unexpected character detected");
                    return;
                }

                line = line.Remove(0, 1);
            }


            System.Console.WriteLine("Final floor: " + floor.ToString());
        }
    }
}