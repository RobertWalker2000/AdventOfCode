using System.Collections.Generic;
namespace AoC_CS
{
    class Y2015_D6T2
    {
        public void Solve()
        {
            int day = 6;
            string filePath = @"Inputs\2015\Day " + day.ToString() + ".txt";

            if (!System.IO.File.Exists(filePath))
            {
                System.Console.WriteLine("ERROR: File does not exist");
                return;
            }
            
            string[] input = System.IO.File.ReadAllLines(filePath);

            int[,] lights = new int[1000, 1000];

            char[] numbers = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

            for (int i = 0; i < input.Length; i++)
            {
                string line = input[i];
                //Parse the instruction to see how to manipulate the lights
                int index = line.IndexOfAny(numbers);
                string instruction = line.Substring(0, index - 1);
                line = line.Remove(0, index);

                //Find the starting co-ordinates for the lights
                Pair<int, int> startPoint = new Pair<int, int>();
                index = line.IndexOf(',');
                string pointString = line.Substring(0, index);
                line = line.Remove(0, index + 1);
                startPoint.X = int.Parse(pointString);

                index = line.IndexOf(' ');
                pointString = line.Substring(0, index);
                line = line.Remove(0, index + 1);
                startPoint.Y = int.Parse(pointString);

                //Skip to the start of the end co-ordinates
                index = line.IndexOfAny(numbers);
                line = line.Remove(0, index);

                //Find the end co-ordinates for the lights
                Pair<int, int> endPoint = new Pair<int, int>();
                index = line.IndexOf(',');
                pointString = line.Substring(0, index);
                line = line.Remove(0, index + 1);
                endPoint.X = int.Parse(pointString);
                endPoint.Y = int.Parse(line);

                switch (instruction)
                {
                    case "turn on":
                        TurnOn(lights, startPoint, endPoint);
                        break;
                    case "turn off":
                        TurnOff(lights, startPoint, endPoint);
                        break;
                    case "toggle":
                        Toggle(lights, startPoint, endPoint);
                        break;
                    default:
                        System.Console.WriteLine("ERROR: Unexpected instruction");
                        return;
                }
            }

            //Count the number of lights that are on
            int lightsCount = 0;
            for(int i = 0; i < 1000; i++)
            {
                for(int j = 0; j < 1000; j++)
                {
                    lightsCount += lights[i, j];
                }
            }
            System.Console.WriteLine("Light Level: " + lightsCount.ToString());
        }

        void TurnOn(int[,] lights, Pair<int, int> start, Pair<int, int> end)
        {
            for(int i = start.X; i <= end.X; i++)
            {
                for(int j = start.Y; j <= end.Y; j++)
                {
                    lights[i, j]++;
                }
            }
        }

        void TurnOff(int[,] lights, Pair<int, int> start, Pair<int, int> end)
        {
            for (int i = start.X; i <= end.X; i++)
            {
                for (int j = start.Y; j <= end.Y; j++)
                {
                    lights[i, j] = System.Math.Max(0, lights[i, j] - 1);
                }
            }
        }

        void Toggle(int[,] lights, Pair<int, int> start, Pair<int, int> end)
        {
            for (int i = start.X; i <= end.X; i++)
            {
                for (int j = start.Y; j <= end.Y; j++)
                {
                    lights[i, j] += 2;
                }
            }
        }
    }
}