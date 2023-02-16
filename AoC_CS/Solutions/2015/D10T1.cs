using System.Collections.Generic;

namespace AoC_CS
{
    class Y2015_D10T1
    {
        public void Solve()
        {
            //Iterations = Length
            //10 = 130
            //20 = 1788
            //30 = 25388
            //40 = 360154

            string startingLine = "1113122113";

            //Read in the set of all possible unique sequences and order them from longest to shortest
            string[] rawInput = System.IO.File.ReadAllLines(@"Inputs\2015\Conways Atoms.txt");
            List<string> atoms = new List<string>();

            foreach(string atom in rawInput)
            {
                //Add the atoms into the list in order of length
                for(int i = 0; i < atoms.Count; i++)
                {
                    if(atom.Length >= atoms[i].Length)
                    {
                        atoms.Insert(i, atom);
                        break;
                    }
                }

                if (!atoms.Contains(atom))
                    atoms.Add(atom);
            }

            //Create a dictionary to track the outputs of each atomic input, and another to track how many of each atom we have
            Dictionary<string, List<string>> processor = new Dictionary<string, List<string>>();
            Dictionary<string, ulong> atomCounter = new Dictionary<string, ulong>();
            foreach (string input in atoms)
            {
                //Add an entry with Key of input, and value of the processed input, split into atoms
                processor.Add(input, SplitAtom(LookSay(input), ref atoms));
                atomCounter.Add(input, 0);
            }

            List<string> startingAtoms = SplitAtom(startingLine, ref atoms);
            foreach (string atom in startingAtoms)
                atomCounter[atom]++;

            for(int i = 0; i < 50; i++)
            {
                //Create a copy of the atom counter as it stands before processing
                Dictionary<string, ulong> inputs = new Dictionary<string, ulong>(atomCounter);

                //reset the counter to 0 for all atoms, ready to count the outputs
                //Use the atoms list as the key as an error is thrown if trying to modify a dictionary while using its own key in a foreach statement
                foreach (string atom in atoms)
                    atomCounter[atom] = 0;

                //Loop through all atoms
                foreach(string atom in inputs.Keys)
                {
                    //Get the atoms produced by processing this input
                    List<string> product = processor[atom];

                    //Count up each new atom from the processed input by incrementing the appropriate atoms by the count of the input
                    foreach (string newAtom in product)
                        atomCounter[newAtom] += inputs[atom];
                }
            }

            //Count up the total length of the final sequence
            ulong length = 0;
            foreach(string atom in atoms)
                length += (ulong)atom.Length * atomCounter[atom];

            System.Console.WriteLine("Final sequence: " + length.ToString());
        }

        List<string> SplitAtom(string input, ref List<string> periodicTable)
        {
            List<string> output = new List<string>();

            //Process the output into its atoms and track them in the dictionary
            while (input.Length > 0)
            {
                foreach (string atom in periodicTable)
                {
                    if (input.StartsWith(atom))
                    {
                        if (input == atom)
                        {
                            output.Add(atom);
                            input = input.Remove(0, atom.Length);
                            break;
                        }

                        //Sequence still contains multiple atoms, ensure we follow the rules for splitting mid sequence
                        //All sequences must start with a 1 or a 3. Exception is the 'Hydrogen' atom (22), which is unique.
                        //All sequences must also end in a 2, except for when followed by a hydrogen atom.

                        if(input[atom.Length - 1] != '2')  //Sequence does not end in a 2, so must be followed by a hydrogen atom to be allowed
                        {
                            if(input[atom.Length] == '2' && input[atom.Length + 1] == '2')
                            {
                                output.Add(atom);
                                input = input.Remove(0, atom.Length);
                                break;
                            }
                            continue;
                        }

                        //Atom ends in a 2, so must be followed by either a 3 or a 1 to be possible
                        if(input[atom.Length] != '2')
                        {
                            output.Add(atom);
                            input = input.Remove(0, atom.Length);
                            break;
                        }
                    }
                }
            }

            return output;
        }

        //Process an input using the LookSay method (The output isthe spoken description of the input)
        //2113221 consists of one 2, two 1s, one 3, two 2s, and one 1. Therefore, the output for that input would be 122113221
        string LookSay(string line)
        {
            string output = "";
            char num = 'x';
            int reps = 0;

            while (line.Length > 0)
            {
                if(line[0] != num)
                {
                    if (reps > 0)
                        output = output + reps.ToString() + num;

                    num = line[0];
                    reps = 0;
                }

                reps++;
                line = line.Remove(0, 1);
            }

            if (reps > 0)
                output = output + reps.ToString() + num;

            return output;
        }
    }
}