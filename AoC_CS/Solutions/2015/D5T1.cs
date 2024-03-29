﻿namespace AoC_CS
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

            //Regex: 1st lookup matches any string with at least 3 vowels(aeiou). 2nd lookup matches for double letters (aa, bb etc.). 3rd lookup excludes any string including a banned phrase (@ab/cd/pq/xy)
            System.Text.RegularExpressions.Regex neededElements = new System.Text.RegularExpressions.Regex(@"(?=.*[aeiou][^aeiou]*[aeiou][^aeiou]*[aeiou])(?=.*(.)\1)^(?!.*(ab|cd|pq|xy))");

            //Count number of lines that match the regex
            int matches = 0;
            foreach (string line in input)
            {
                    if (neededElements.IsMatch(line))
                        matches++;
            }
            
            System.Console.WriteLine("Nice words: " + matches.ToString());
        }
    }
}