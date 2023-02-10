using System.Collections.Generic;

namespace AoC_CS
{
    class Y2015_D7T1
    {
        public void Solve()
        {
            int day = 7;
            string filePath = @"Inputs\2015\Day " + day.ToString() + ".txt";

            if (!System.IO.File.Exists(filePath))
            {
                System.Console.WriteLine("ERROR: File does not exist");
                return;
            }
            
            string[] input = System.IO.File.ReadAllLines(filePath);

            //Read in each node and store it
            SortedDictionary<string, CircuitNode> nodes = new SortedDictionary<string, CircuitNode>();
            foreach(string line in input)
                NodeParser.Parse(line, ref nodes);

            //Evaluate our necessary node
            int answer = nodes["a"].Evaluate(ref nodes);

            System.Console.WriteLine("Signal to wire a: " + answer.ToString());
        }
    }

    class CircuitNode
    {
        public enum Operations { DIRECT, AND, OR, NOT, LSHIFT, RSHIFT };

        int value;
        public bool evaluated { get; private set; } = false;
        public Operations opType;
        public string lhs;
        public string rhs;


        public int Evaluate(ref SortedDictionary<string, CircuitNode> nodes)
        {
            //If we already have a value, no need to calculate it again
            if (evaluated)
                return value;

            //Try and parse the lhs directly into a value. If not possible, evaluate the corresponding node
            int lVal;
            if(!int.TryParse(lhs, out lVal))
                lVal = nodes[lhs].Evaluate(ref nodes);

            //As above, but for right node
            int rVal;
            if (!int.TryParse(rhs, out rVal))
                rVal = nodes[rhs].Evaluate(ref nodes);

            //Calculate the value of this node using our other 2 values and the operation type
            switch(opType)
            {
                case Operations.DIRECT:
                    value = lVal;
                    break;
                case Operations.AND:
                    value = lVal & rVal;
                    break;
                case Operations.OR:
                    value = lVal | rVal;
                    break;
                case Operations.NOT:
                    value = ~ lVal;
                    break;
                case Operations.LSHIFT:
                    value = lVal << rVal;
                    break;
                case Operations.RSHIFT:
                    value = lVal >> rVal;
                    break;
                default:
                    System.Console.WriteLine("ERROR: Unexpected operations type during evaluation");
                    return 0;
            }

            evaluated = true;
            return value;
        }
    }

    static class NodeParser
    {
        static char[] keyLetters = { 'A', 'O', 'N', 'L', 'R' };

        public static void Parse(string input, ref SortedDictionary<string, CircuitNode> dict)
        {
            CircuitNode node = new CircuitNode();

            //Find and set the nodes name
            int index = input.IndexOf('>') + 2;
            string name = input.Substring(index);

            //Find the nodes operation type and process it accordingly
            index = input.IndexOfAny(keyLetters);
            if(index == -1)
            {
                node.opType = CircuitNode.Operations.DIRECT;
                ParseDirect(input, ref node);
                dict.Add(name, node);
                return;
            }

            char opChar = input[index];

            switch(opChar)
            {
                case 'N':
                    node.opType = CircuitNode.Operations.NOT;
                    ParseNot(input, ref node);
                    dict.Add(name, node);
                    return;
                case 'A':
                    node.opType = CircuitNode.Operations.AND;
                    break;
                case 'O':
                    node.opType = CircuitNode.Operations.OR;
                    break;
                case 'L':
                    node.opType = CircuitNode.Operations.LSHIFT;
                    break;
                case 'R':
                    node.opType = CircuitNode.Operations.RSHIFT;
                    break;
                default:
                    System.Console.WriteLine("ERROR: Unexpected operation type " + opChar);
                    return;
            }

            ParseCompare(input, ref node);
            dict.Add(name, node);
            return;
        }

        static void ParseDirect(string input, ref CircuitNode node)
        {
            int index = input.IndexOf(' ');
            node.lhs = input.Substring(0, index);
            node.rhs = input.Substring(0, index);
        }

        static void ParseCompare(string input, ref CircuitNode node)
        {
            //Read in lhs and delete from string
            int index = input.IndexOf(' ');
            node.lhs = input.Substring(0, index);
            input = input.Remove(0, index + 1);

            //Delete the compare type
            index = input.IndexOf(' ');
            input = input.Remove(0, index + 1);

            //Read in the rhs
            index = input.IndexOf(' ');
            node.rhs = input.Substring(0, index);
        }

        static void ParseNot(string input, ref CircuitNode node)
        {
            //Remove all information other than the value/node name
            input = input.Remove(0, input.IndexOf(' ') + 1);
            int index = input.IndexOf(' ');
            input = input.Remove(input.IndexOf(' '));
            node.lhs = input;
            node.rhs = input;
        }
    }
}