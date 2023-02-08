namespace AoC_CS
{
    class Y2015_D0T0
    {
        public void Solve()
        {
            int day = 0;
            string filePath = @"Inputs\2015\Day " + day.ToString() + ".txt";

            if (!System.IO.File.Exists(filePath))
            {
                System.Console.WriteLine("ERROR: File does not exist");
                return;
            }
            
            string[] input = System.IO.File.ReadAllLines(filePath);

            System.Console.WriteLine("Output Answer Here!");
        }
    }
}