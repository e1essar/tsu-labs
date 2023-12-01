
namespace List_and_Set
{
    class Set : List
    {
        private int power = 0;
        public int Get_Power() { return power; }    
        public Set(int? Data) : base(Data) { power = 1; }
        public Set() : base() { }
        public Set(Set A) : base(A) { power = A.Size(); }
        public Set(List A) {

            if (A.Get_Head() == null) { Set_Head(null); return; }

            Set_Head(new Node(A.Get_Head().Get_Data()));
            Node current = A.Get_Head().Get_Next();

            while (current != null)
            {
                if(Find(current.Get_Data()) != null)
                {
                    current = current.Get_Next();
                }
                else
                {
                    Add_To_Tail(current.Get_Data());
                    current = current.Get_Next();
                }
            }
            power = this.Size();
        }

        public void Input_Set()
        {
            if (Get_Head() != null) Clear();
            
            Console.Write("Enter a power of set: ");
            power = int.Parse(Console.ReadLine());
            
            Console.WriteLine("Enter values: ");
            Node current1 = new Node(int.Parse(Console.ReadLine()));
            
            Set_Head(current1);
            Node current2 = current1;
            int tmp;
            for (int i = 1; i < power; i++)
            {
                tmp = int.Parse(Console.ReadLine());
                while (current2 != null)
                {
                    if (tmp == current2.Get_Data())
                    {
                        Console.WriteLine("Repetition! Enter another one");
                        tmp = int.Parse(Console.ReadLine());
                        current2 = Get_Head();
                        continue;
                    }
                    current2 = current2.Get_Next();
                }
                current1.Set_Next(new Node(tmp));
                current1 = current1.Get_Next();
                current2 = Get_Head();
            }
        }

        public void Output_Set() { Output_List(); }

        public static bool operator == (Set A, Set B)
        {
            A.Sort();
            B.Sort();

            Node? current1 = A.Get_Head();
            Node? current2 = B.Get_Head();
            while (current1 != null)
            {
                if (current1.Get_Data() != current2.Get_Data()) return false;
                current1 = current1.Get_Next();
                current2 = current2.Get_Next();
            }
            return true;
        }

        public static bool operator != (Set A, Set B)
        {
            return !(A == B);
        }

        public Set Unity(Set A)
        {
            Node? current = A.Get_Head();

            Set result = new Set(this);

            while (current != null)
            {
                if (Find(current.Get_Data()) != null)
                {
                    current = current.Get_Next();
                }
                else
                {
                    result.Add_To_Tail(current.Get_Data());
                    current = current.Get_Next();
                }
            }
            result.power = result.Size();
            return result;

        }

        public void Add_To_Set(int? data)
        {
            if (Find(data) == null)
            {
                Add_To_Tail(data);
            }
            else
            {
                Console.WriteLine("Data is already in set");
            }
            power = this.Size();
        }

        public Set Intersection(Set A)
        {
            Node? current = A.Get_Head();

            Set result = new Set();

            while (current != null)
            {
                if (Find(current.Get_Data()) != null)
                {
                    result.Add_To_Tail(current.Get_Data());
                    current = current.Get_Next();
                }
                else
                {
                    current = current.Get_Next();
                }
            }
            result.power = result.Size();
            return result;
        }

        public Set Difference(Set A)
        {
            Node? current = A.Get_Head();

            Set result = new Set(this);

            while (current != null)
            {
                if (Find(current.Get_Data()) != null)
                {
                    result.Remove_By_Key(current.Get_Data());
                    current = current.Get_Next();
                }
                else
                {
                    current = current.Get_Next();
                }
            }
            result.power = result.Size();
            return result;
        }

        public void Remove_From_Set(int? data)
        {
            Remove_By_Key(data);
            power--;
        }

        public Set Complement()
        {
            Set result = new Set();
            int? size = Max();
            int? i = Min() + 1;

            for(;i<size;i++)
            {
                if(Find(i) == null)
                {
                    result.Add_To_Tail(i);
                }
            }
            result.power = result.Size();
            return result;
            
        }

    }
}
