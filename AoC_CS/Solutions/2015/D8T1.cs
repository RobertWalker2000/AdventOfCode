namespace AoC_CS
{
    class Y2015_D8T1
    {
        public void Solve()
        {
            int day = 8;
            string filePath = @"Inputs\2015\Day " + day.ToString() + ".txt";

            if (!System.IO.File.Exists(filePath))
            {
                System.Console.WriteLine("ERROR: File does not exist");
                return;
            }
            
            string[] input = System.IO.File.ReadAllLines(filePath);

            int score = 0;
            foreach(string line in input)
            {
                score += ScoreString(line);
            }

            System.Console.WriteLine("Final Score: " + score.ToString());
        }

        static int ScoreString(string line)
        {
            int stringChars = 0;
            int memoryChars = 0;
            char[] chars = line.ToCharArray();

            for(int i = 0; i < chars.Length; i++)
            {
                char c = chars[i];
                //All letter characters are processed as 1 stringChar and 1 memoryChar
                if(c >= 'a' && c <= 'z')
                {
                    stringChars++;
                    memoryChars++;
                    continue;
                }

                //Check if we are dealing with a default " character, marking the start/end of the string
                if(c == '\"')
                {
                    stringChars++;
                    continue;
                }

                //Check we have the start of an escape sequence and find its type
                if(c == '\\')
                {
                    char d = chars[i + 1];

                    if(d == '\"' || d == '\\')
                    {
                        //A quotation mark or backslash in the middle of the string uses 2 characters, but only needs 1 space in memory
                        stringChars += 2;
                        memoryChars += 1;
                        i++;
                        continue;
                    }
                    else if (d == 'x')
                    {
                        //A section of the form "\x12" uses 4 characters in the string, but only represents one character in memory
                        stringChars += 4;
                        memoryChars += 1;
                        i += 3;
                        continue;
                    }
                    else
                    {
                        System.Console.WriteLine("ERROR: Unexpected escape sequence");
                        break;
                    }
                }
            }

            return stringChars - memoryChars;
        }
    }

}