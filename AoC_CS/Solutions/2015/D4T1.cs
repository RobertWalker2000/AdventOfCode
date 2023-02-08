namespace AoC_CS
{
    class Y2015_D4T1
    {
        public void Solve()
        {
            string input = "iwrupvqb";

            int i = 0;
            while (i < int.MaxValue)
            {
                //Create the hash
                string hash = CreateHash(input + i.ToString());

                //Check if the hash has 5 leading 0s. If so, exit with our answer
                string sub = hash.Substring(0, 6);
                if (sub == "000000")
                    break;

                //If not 5 leading 0s, try again with the next number
                i++;
            }

            System.Console.WriteLine("Lowest compatible number: " + i.ToString());
        }

        static string CreateHash(string input)
        {
            //Create the hashing object
            System.Security.Cryptography.MD5 md5 = System.Security.Cryptography.MD5.Create();

            //Convert the string to a bytestream for the hashing algorithm
            byte[] inputBytes = System.Text.Encoding.ASCII.GetBytes(input);

            //Generate hash from bytestream
            byte[] hashBytes = md5.ComputeHash(inputBytes);

            string output = "";
            //Convert hash to hex format, only do the first 3 bytes as these are the only relevant parts
            for(int i = 0; i < hashBytes.Length; i++)
            {
                int num = System.Convert.ToInt32(hashBytes[i]);
                string hex = num.ToString("X2");
                output += hex;
            }

            return output;
        }
    }
}