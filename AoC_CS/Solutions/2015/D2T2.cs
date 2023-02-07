namespace AoC_CS
{
    class D2T2
    {
        public void Solve()
        {
            int day = 2;
            string filePath = @"Inputs\2015\Day " + day.ToString() + ".txt";

            if (!System.IO.File.Exists(filePath))
            {
                System.Console.WriteLine("ERROR: File does not exist");
                return;
            }
            
            string[] input = System.IO.File.ReadAllLines(filePath);

            int total = 0;
            foreach (string line in input)
            {
                string[] dimensions = line.Split('x');
                int l = int.Parse(dimensions[0]);
                int w = int.Parse(dimensions[1]);
                int h = int.Parse(dimensions[2]);

                //Smallest perimeter is the sum of the 2 shortest sides. Add all sides then remove the largest
                int smallPerimeter = l + h + w;
                smallPerimeter -= System.Math.Max(l, System.Math.Max(h, w));
                smallPerimeter *= 2;

                int volume = l * h * w;

                int ribbonNeeded = volume + smallPerimeter;
                total += ribbonNeeded;
            }

            System.Console.WriteLine("Ribbon Needed: " + total + "ft.");
        }
    }
}