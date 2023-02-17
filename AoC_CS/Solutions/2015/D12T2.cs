namespace AoC_CS
{
    class Y2015_D12T2
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
            string json = input[0];

            //Strip out all objects which contain the property "red"
            int index = 0;
            while (index >= 0)
            {
                //Find the next instance of "red" in the string
                index = json.IndexOf("\"red\"", index);

                if (index < 0)
                    break;

                if(IsObject(json, index, out int start, out int end))
                {
                    int len = (end - start) + 1;
                    json = json.Remove(start, len);
                    index = start;
                    continue;
                }

                //Increment the index so we don't capture the same "red" again
                index++;
            }

            //Use a RegEx to get all the positive and negative numbers in the sequence
            var matches = System.Text.RegularExpressions.Regex.Matches(json, @"\d+|-\d+");

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

        //Returns true if the given "center" point is inside an object
        //Also returns the start and end positions of the object using the out keyword
        bool IsObject(string s, int center, out int start, out int end)
        {
            start = center;
            end = center;

            //Variable to track how many layers deep we are in either a list or an object
            int list = 0;
            int obj = 0;

            //Figure out if we are in an array or an object (or neither)
            //Do this by checking for open/closing brackets after our instance of "red"
            //The first time we find more closing than opening brackets, we know what we are in.
            //If we don't find such an instance, we are not in either
            int i = center;
            while(obj >= 0)
            {
                //Get the positions of the next instance for each bracket type
                int listOpen = s.IndexOf('[', i + 1);
                int listClose = s.IndexOf(']', i + 1);
                int objOpen = s.IndexOf('{', i + 1);
                int objClose = s.IndexOf('}', i + 1);

                //Find out which of the brackets was first in the order
                if (WasFirst(listOpen, new int[] { listClose, objOpen, objClose }))
                {
                    list++;
                    i = end = listOpen;
                }
                else if (WasFirst(listClose, new int[] { listOpen, objOpen, objClose }))
                {
                    list--;
                    i = end = listClose;
                }
                else if (WasFirst(objOpen, new int[] { listOpen, listClose, objClose }))
                {
                    obj++;
                    i = end = objOpen;
                }
                else if (WasFirst(objClose, new int[] { listOpen, listClose, objOpen }))
                {
                    obj--;
                    i = end = objClose;
                }
                else
                    return false;   //If we reach here, all brackets were invalid and we aren't in a array or object

                //If we are in a list, stop processing
                if (list < 0)
                    return false;

                //If we are in an object, need to find the start of the object
                if (obj < 0)
                    break;
            }

            //We know we are in an object. Now find the start of this object
            obj = 0;
            i = center;
            while(obj >= 0) //Loop until we find the bracket that opens the object we are currently in
            {
                //Find the first instance of an object bracket, working backwards form the current index
                int open = s.LastIndexOf('{', i - 1);
                int close = s.LastIndexOf('}', i - 1);

                //If the first bracket is an open, reduce the obj count
                if (open > close)
                {
                    obj--;
                    i = start = open;
                    continue;
                }

                //The first bracket was a close, so increase the object count
                obj++;
                i = start = close;
            }

            return true;
        }

        bool WasFirst(int index, int[] others)
        {
            if (index < 0)
                return false;

            for(int i = 0; i < others.Length; i++)
            {
                if (others[i] < 0)
                    continue;

                if (index > others[i])
                    return false;
            }

            return true;
        }
    }
}