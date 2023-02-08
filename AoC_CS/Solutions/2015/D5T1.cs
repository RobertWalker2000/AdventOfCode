namespace AoC_CS
{
    class Y2015_D5T1
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

            //Regex expression to match any string which contains any of the pairs ab/cd/pq/xy
            System.Text.RegularExpressions.Regex bannedWords = new System.Text.RegularExpressions.Regex(@"ab|cd|pq|xy");

            //Regex expression to match any string containing at least 3 vowels, and also has any consecutively repeated letter
            System.Text.RegularExpressions.Regex neededElements = new System.Text.RegularExpressions.Regex(@"(?=.*[aeiou][^aeiou]*[aeiou][^aeiou]*[aeiou])(?=.*(.)\1)");

            int matches = 0;
            foreach (string line in input)
            {
                if (!bannedWords.IsMatch(line))
                    if (neededElements.IsMatch(line))
                        matches++;
            }
            
            System.Console.WriteLine("Nice words: " + matches.ToString());
        }
    }
}