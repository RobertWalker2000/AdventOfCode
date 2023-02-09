namespace AoC_CS
{
    class Pair<T1, T2> : System.IComparable
    {
        public Pair()
        {
            X = default(T1);
            Y = default(T2);
        }

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

        public int CompareTo(object obj)
        {
            if (obj == null)
                return 1;

            //Check that the other object is of the same type as this
            Pair<T1, T2> other = obj as Pair<T1, T2>;
            if (other == null)
                throw new System.ArgumentException("Object was not of a matching Pair type");

            //If X values are equal, compare Y values
            if(this.X.Equals(other.X))
                return System.Collections.Generic.Comparer<T2>.Default.Compare(this.Y, other.Y);


            return System.Collections.Generic.Comparer<T1>.Default.Compare(this.X, other.X);
        }
    }
}