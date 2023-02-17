namespace AoC_CS
{
    class Y2015_D12T1
    {
        public void Solve()
        {
            int day = 12;
            string filePath = @"Inputs\2015\Day " + day.ToString() + ".txt";

            if (!System.IO.File.Exists(filePath))
            {
                System.Console.WriteLine("ERROR: File does not exist");
                return;
            }
            
            string[] input = System.IO.File.ReadAllLines(filePath);

            //Use a RegEx to get all the positive and negative numbers in the sequence
            var matches = System.Text.RegularExpressions.Regex.Matches(input[0], @"\d+|-\d+");

            int totalSum = 0;
            for(int i = 0; i < matches.Count; i++)
            {
                string s = matches[i].Value;

                if (int.TryParse(s, out int num))
                    totalSum += num;
                else
                    System.Console.WriteLine("ERROR: Could not parse string to int: " + s);
            }

            System.Console.WriteLine("Total sum: " + totalSum.ToString());
        }
    }
}