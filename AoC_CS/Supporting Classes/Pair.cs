namespace AoC_CS
{
    class Pair<T1, T2>
    {
        public Pair(T1 val1, T2 val2)
        {
            X = val1;
            Y = val2;
        }

        public T1 X { get; set; }
        public T2 Y { get; set; }

        public override bool Equals(object obj)
        {
            if(obj is Pair<T1, T2>)
            {
                //Unbox object to pair
                Pair<T1, T2> other = (Pair<T1, T2>)obj;

                //If both values are equal, return true; Otherwise, return false
                return this.X.Equals(other.X) && this.Y.Equals(other.Y);
            }

            return false;
        }


        public override int GetHashCode()
        {
            return X.GetHashCode() * Y.GetHashCode();
        }
    }
}