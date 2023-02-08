namespace AoC_CS
{
    class Y2015_D5T2
    {
        public void Solve()
        {
            int day = 5;
            string filePath = @"Inputs\2015\Day " + day.ToString() + ".txt";

            if (!System.IO.File.Exists(filePath))
            {
                System.Console.WriteLine("ERROR: File does not exist");
                return;
            }
            
            string[] input = System.IO.File.ReadAllLines(filePath);

            //Regex: First lookup matches repeated pairs of letters (e.g. erfer). Second lookup matches repeated letters one space apart(e.g. aba or aaa)
            System.Text.RegularExpressions.Regex rx = new System.Text.RegularExpressions.Regex(@"(?=.*(..).*\1)(?=.*(.).\2)");

            //Count the number of lines that match the regex
            int matches = 0;
            foreach (string line in input)
            {
                    if (rx.IsMatch(line))
                        matches++;
            }
            
            System.Console.WriteLine("Nice words: " + matches.ToString());
        }
    }
}