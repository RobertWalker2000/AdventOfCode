namespace AoC_CS
{
    class Y2015_D11T1
    {
        public void Solve()
        {
            char[] password = "hxbxwxba".ToCharArray();

            System.Text.RegularExpressions.Regex doubleDoubles = new System.Text.RegularExpressions.Regex(@"(?=.*(.)\1.*(.)\2)");   //Regex to check for 2 sets of double letters

            //Start by incrementing the password to its first iteration with a sequence
            int seqStart = password.Length - 3;

            int passwordIterations = 0;
            const int neededIterations = 1;
            while(passwordIterations < neededIterations)
            {
                //Increment the password
                IncrementChar(password, password.Length - 1);

                //If we don't have a valid sequence where expected, get the next viable sequence
                if (!ContainsSequence(password))
                {
                    //If we have a double letter in our password, the sequence may be later in the password than our previous attempt
                    //Go back to the end and start searching for fresh sequences
                    //if (singleDouble.IsMatch(new string(password)))
                    seqStart = password.Length - 3;

                    //Find the next password with a sequence
                    NextSequence(password, ref seqStart);
                }

                //Check if the current password contains 2 sets of double letters
                if (doubleDoubles.IsMatch(new string(password)))
                    passwordIterations++;
            }

            System.Console.WriteLine("Password: " + new string(password));
        }

        void NextSequence(char[] input, ref int seqIndex)
        {
            //Check that we can make a sequence starting at the current position
            //Sequence is impossible if starting char is y or z, as this would cause the final char to be out of bounds
            if(input[seqIndex] >= 'y')
            {
                //Move back a character and try again
                seqIndex--;
                NextSequence(input, ref seqIndex);
                return;
            }

            //If the 2nd char is more than 1 greater than the 1st, can't make a sequence without incrementing the first character
            if(input[seqIndex + 1] > input[seqIndex] + 1)
            {
                IncrementChar(input, seqIndex);

                if (ContainsSequence(input))
                    return;

                seqIndex = input.Length - 3;    //Reset the sequence index to check for sequences in front of the char we just incremented, since we have a fresh set to work with
                NextSequence(input, ref seqIndex);
                return;
            }

            //If the 2nd char is already in sequence, the 3rd char must be no greater than the 2nd to allow a sequence
            //If the 3rd char is too high in this instance, we must increment the 1st char to give room fro the 2nd char to incrmeent and stay in sequence
            if((input[seqIndex + 1] == input[seqIndex] + 1) && (input[seqIndex + 2] > input[seqIndex + 1]))
            {
                IncrementChar(input, seqIndex);

                if (ContainsSequence(input))
                    return;

                seqIndex = input.Length - 3;    //Reset the seqIndex to check for sequences in front of this, as we have a new set to work with
                NextSequence(input, ref seqIndex);
                return;
            }

            //If we reached here, all characters are appropriate to generate a sequence
            //Set the 2 characters following the sequence start to be ascending
            input[seqIndex + 1] = (char)(input[seqIndex] + 1);

            //Updating the 2ndchar may have created a new sequence, check for that
            if (ContainsSequence(input))
                seqIndex--;
            else
                input[seqIndex + 2] = (char)(input[seqIndex] + 2);

            //Set all characters after the sequence to be 'a'
            for (int i = seqIndex + 3; i < input.Length; i++)
                input[i] = 'a';

            return;
        }

        //Checks if the string contains 3 sequential characters anywehere within it
        bool ContainsSequence(char[] password)
        {
            int seqLength = 0;
            char lastChar = ' ';
            for (int i = 0; i < password.Length; i++)
            {
                if (lastChar + 1 != password[i])
                    seqLength = 0;

                lastChar = password[i];
                seqLength++;

                if (seqLength >= 3)
                    return true;
            }

            return false;
        }

        //Increments the char at a given index and all of its rollovers
        void IncrementChar(char[] input, int index)
        {
            //Start at the character to be incremented, working backwards whenever a character wraps around
            for(int i = index; i >= 0; i--)
            {
                input[i]++;

                if(input[i] > 'z')
                {
                    input[i] = 'a';
                    continue;
                }

                //Don't allow banned letters
                if(input[i] == 'i' || input[i] == 'l' || input[i] == 'o')
                    input[i]++;

                //If we didn't wrap around, don't move on to the next character
                break;  
            }

            //Reset everything after the index to be 'a', as they would all have needed to wrap around to increment the indexed char
            for(int i = index + 1; i < input.Length; i++)
                input[i] = 'a';
        }
    }

}