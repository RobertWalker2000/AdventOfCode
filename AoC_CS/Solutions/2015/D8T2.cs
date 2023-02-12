namespace AoC_CS
{
    class Y2015_D8T2
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
            int encodedChars = 0;
            char[] chars = line.ToCharArray();

            for(int i = 0; i < chars.Length; i++)
            {
                char c = chars[i];
                //All letter characters are processed as 1 stringChar and 1 memoryChar. No additional encoding necessary
                if(c >= 'a' && c <= 'z')
                {
                    stringChars++;
                    encodedChars++;
                    continue;
                }

                //Check if we are dealing with a default " character, marking the start/end of the string
                if(c == '\"')
                {
                    //The outer " marks are encoded as "\ and therefore take 2 encoded chars
                    stringChars++;
                    encodedChars += 2;
                    continue;
                }

                //Check we have the start of an escape sequence and find its type
                if(c == '\\')
                {
                    char d = chars[i + 1];

                    if(d == '\"' || d == '\\')
                    {
                        //A quotation mark or backslash in the middle of the string uses 2 characters, but needs 4 encoded chars (one for the 1st slash and one for the second slash or quote mark)
                        stringChars += 2;
                        encodedChars += 4;
                        i++;
                        continue;
                    }
                    else if (d == 'x')
                    {
                        //A section of the form "\x12" uses 4 characters in the string, but needs an extra # \ for encoding, so takes 5 encoded chars
                        stringChars += 4;
                        encodedChars += 5;
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

            //Add an extra 2 chars to the encoded counter for the new " marks at the start/end of the string
            encodedChars += 2;

            return encodedChars - stringChars;
        }
    }

}