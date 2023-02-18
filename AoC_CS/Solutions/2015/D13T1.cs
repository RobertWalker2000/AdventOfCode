using System.Collections.Generic;

namespace AoC_CS
{
    class Y2015_D13T1
    {
        public void Solve()
        {
            int day = 13;
            string filePath = @"Inputs\2015\Day " + day.ToString() + ".txt";

            if (!System.IO.File.Exists(filePath))
            {
                System.Console.WriteLine("ERROR: File does not exist");
                return;
            }

            string[] input = System.IO.File.ReadAllLines(filePath);

            //Record all the happiness changes for each possible pairing
            Dictionary<string, Person> people = new Dictionary<string, Person>();
            foreach (string line in input)
            {
                string subject, partner;
                int happiness;

                //Get the first name in the sequence
                int index = line.IndexOf(' ');
                subject = line.Substring(0, index);

                //Get the last name in the sequence
                index = line.LastIndexOf(' ');
                partner = line.Substring(index + 1);
                partner = partner.TrimEnd('.');

                //Use a RegEx to extract the number from the string, then conmvert it to string, then convert it to an int
                happiness = int.Parse(System.Text.RegularExpressions.Regex.Match(line, @"\d+").ToString());

                //If the line mentions losing happiness, happiness should be -ve
                if (line.Contains("lose"))
                    happiness = -happiness;

                //Now that we have all the details, create or update the subjects entry in the dictionary with the appropriate pairing
                if (!people.ContainsKey(subject))
                    people.Add(subject, new Person());

                people[subject].AddPairing(partner, happiness);
            }

            //Assign each person a number to allow for easier creation of all seating arrangements
            Dictionary<int, string> ID = new Dictionary<int, string>();
            int x = 0;
            foreach (string name in people.Keys)
            {
                ID.Add(x, name);
                x++;
            }

            //Construct the starting order for checking pairings
            int[] order = new int[people.Count];
            for(int i = 0; i < people.Count; i++)
                order[i] = i;

            //As this is a circular layout, we don't need to try each person at the start of the list
            //Any layouts produced by changing the starting element will have already been checked. The only difference would be everyone being shifted round 1 place, but the pairings would all be the same
            int maxHappiness = int.MinValue;
            int maxWithMe = int.MinValue;

            while(order[0] == 0)
            {
                //Calculate the total happiness of this layout by calculating the total happiness of each pair

                //Calculate the pairing between the first and last entry as this wraps around the array
                string p1 = ID[order[order.Length - 1]];
                string p2 = ID[order[0]];
                int pairHap = people[p1].GetHappiness(p2);
                pairHap += people[p2].GetHappiness(p1);
                int badPair = pairHap;
                int layoutHap = pairHap;

                //Calculate the happiness for each other pairing
                for(int i = 0; i < order.Length - 1; i++)
                {
                    p1 = ID[order[i]];
                    p2 = ID[order[i + 1]];
                    pairHap = people[p1].GetHappiness(p2);
                    pairHap += people[p2].GetHappiness(p1);
                    layoutHap += pairHap;

                    //If this pairing has the worst happiness for this layout, save it
                    badPair = System.Math.Min(badPair, pairHap);
                }

                //Track the overall best layout
                maxHappiness = System.Math.Max(maxHappiness, layoutHap);

                //Track the best layout which includes me, by removing the worst pair from the total happiness to simulate me being slotted between the two people
                maxWithMe = System.Math.Max(maxWithMe, layoutHap - badPair);

                NextOrder(order);
            }
            
            System.Console.WriteLine("Best layout: " + maxHappiness.ToString());
            System.Console.WriteLine("Best layout with me: " + maxWithMe.ToString());
        }

        //Finds the next order possible when incrementing the given element in the array
        void NextOrder(int[] order, int startPoint)
        {
            //Track the value of the relevant entry for later
            int startValue = order[startPoint];

            //Clear out any values after and including the start point
            for (int i = startPoint; i < order.Length; i++)
                order[i] = -1;

            //Now loop through each empty element, starting with the earliest, and set it to the lowest possible
            for (int i = startPoint; i < order.Length; i++)
            {
                //Increment the new value until we find a valid entry
                for (int newVal = startValue + 1; newVal < order.Length; newVal++)
                {
                    if (!ArrayContains<int>(order, newVal))
                    {
                        order[i] = newVal;
                        startValue = -1;
                        break;
                    }
                }
                //If we did not find a viable entry for the current element, we must increment the entry before it instead
                if (order[i] < 0)
                {
                    NextOrder(order, startPoint - 1);
                    return;
                }
            }
        }

        //Finds the next possible order, starting by incrementing
        void NextOrder(int[] order)
        {
            NextOrder(order, order.Length - 1);
        }

        bool ArrayContains<T>(T[] arr, T val)
        {
            for(int i = 0; i < arr.Length; i++)
            {
                if (arr[i].Equals(val))
                    return true;
            }

            return false;
        }
    }

    class Person
    {
        public Person() { }

        Dictionary<string, int> pairingHappiness = new Dictionary<string, int>();

        public void AddPairing(string name, int happiness)
        {
            if (!pairingHappiness.ContainsKey(name))
                pairingHappiness.Add(name, 0);

            pairingHappiness[name] = happiness;
        }

        //Returns the happiness for this person based on a given partner
        public int GetHappiness(string partner)
        {
            if (!pairingHappiness.ContainsKey(partner))
                return 0;

            return pairingHappiness[partner];
        }
    }
}