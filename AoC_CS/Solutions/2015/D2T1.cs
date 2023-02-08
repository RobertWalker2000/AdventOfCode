namespace AoC_CS
{
    class Y2015_D2T1
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

                int front = l * h;
                int top = l * w;
                int side = w * h;
                int bonus = System.Math.Min(front, System.Math.Min(top, side));

                int area = (2 * front) + (2 * top) + (2 * side) + bonus;
                total += area;
            }

            System.Console.WriteLine("Paper Needed: " + total + "sq. ft.");
        }
    }
}