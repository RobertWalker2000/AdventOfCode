namespace AoC_CS
{
    class Y2015_D4T2
    {
        public void Solve()
        {
            string input = "iwrupvqb";

            int i = 0;

            //Create the hashing object
            System.Security.Cryptography.MD5 md5 = System.Security.Cryptography.MD5.Create();
            byte[] hash = new byte[16];
            while (i < int.MaxValue)
            {
                //Convert the string to a bytestream for the hashing algorithm
                hash = md5.ComputeHash(System.Text.Encoding.ASCII.GetBytes(input + i.ToString()));

                if (hash[0] == 0 && hash[1] == 0 && hash[2] == 0)
                    break;

                //If not 5 leading 0s, try again with the next number
                i++;
            }
            System.Console.WriteLine("Lowest compatible number: " + i.ToString());
        }
    }
}