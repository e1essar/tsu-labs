
namespace List_and_Set
{
    class List
    {
        private Node? head;
        // constructors
        public List() { head = null; } 
        public List(int? Data) { head = new Node(Data); }
        public List(List A)
        {
            if (A.head == null) { head = null; return; }
            
            head = new Node(A.head.Get_Data());
            Node? current = A.head.Get_Next();

            while (current != null)
            {
                Add_To_Tail(current.Get_Data());
                current = current.Get_Next();
            }
        }
        public Node? Get_Head() { return head; }
        public void Set_Head(Node Head) { head = Head; }
        // methods
        public Node? Find(int? key)
        {
            if (head == null) { return null; }

            Node? current = head;

            while(current != null)
            {
                if (current.Get_Data() == key)
                {
                    return current;
                }
                current = current.Get_Next();
            }
            return null;
        }
        public void Input_List()
        {
            Clear();

            Console.Write("List size: ");
            int size = int.Parse(Console.ReadLine());
            Console.Write("Enter values: ");
            Node? current = new Node(int.Parse(Console.ReadLine()));
            head = current;
            for (int i = 1; i < size; i++)
            {
                current.Set_Next(new Node(int.Parse(Console.ReadLine())));
                current = current.Get_Next();
            }
        }
        public void Output_List()
        {
            //Console.WriteLine("Output: ");
            Node? current = head;

            while (current != null)
            {
                Console.Write(current.Get_Data());
                Console.Write(" ");
                current = current.Get_Next();
            }

            Console.WriteLine();
        }
        public void Sort() 
        {
           if (head == null || head.Get_Next() == null) { return;}

             bool flag = true;
             Node? current = head;
             Node? next = head.Get_Next();
             Node? tmp = new Node(0);

             while (flag)
             {
                 flag = false;
                 while (next != null)
                 {
                     if (current.Get_Data() > next.Get_Data())
                     {
                         tmp.Set_Data(current.Get_Data());
                         current.Set_Data(next.Get_Data());
                         next.Set_Data(tmp.Get_Data());
                         flag = true;
                     }
                     current = current.Get_Next();
                     next = next.Get_Next();
                 }
                 current = head;
                 next = head.Get_Next();
             }
            
        }
        public void Add_To_Head(int? data)
        {
            if (head == null) { head = new Node(data); return; }

            Node node = new Node(data);
            node.Set_Next(head);
            head = node;
        }
        public void Add_To_Tail(int? data)
        {
            if (head == null) { head = new Node(data); return; }

            Node? current = head;
            while (current.Get_Next() != null) current = current.Get_Next();
            current.Set_Next(new Node(data));
        }
        public void Add_To(int? data, int? pos)
        {
            if (pos <= 0 || pos > Size()) { Console.WriteLine("Out of range!"); return; }

            if (pos == 1) { Add_To_Head(data); return; }

            Node node = new Node(data);
            Node? current = head;
            Node? following = head.Get_Next();
            int size = Size();

            for (int i = 1; i < size; i++)
            {
                if (i == pos - 1)
                {
                    current.Set_Next(node);
                    node.Set_Next(following);
                    break;
                }
                following = following.Get_Next();
                current = current.Get_Next();
            }
        }
        public void Add_After_Key(int? data, int? key)
        {
            Node node = new Node(data);
            Node current = head;
            Node following = head.Get_Next();
            int size = Size();
            while (current.Get_Next() != null)
            {
                if (key == current.Get_Data())
                {
                    current.Set_Next(node);
                    node.Set_Next(following);
                    break;
                }
                if (following != null)
                {
                    following = following.Get_Next();
                    current = current.Get_Next();
                }
                else
                { Console.WriteLine("Key was not found"); return; }
            }
        }
        public void Remove_From_Head()
        {
            head = head.Get_Next();
        }
        public void Remove_From_Tail()
        {
            Node? current = head;
            if (current.Get_Next() == null) return;
            while (current.Get_Next().Get_Next() != null) current = current.Get_Next();

            current.Set_Next(null);
        }
        public void Remove_From(int pos)
        {
            if (pos <= 0 || pos > Size()) { Console.WriteLine("Out of range!"); return; }

            if (pos == 1) { Remove_From_Head(); return; }

            Node current = head;
            Node following = head.Get_Next();
            int size = Size();
            for (int i = 1; i < size; i++)
            {
                if (i == pos - 1)
                {
                    current.Set_Next(following.Get_Next());
                    break;
                }
                following = following.Get_Next();
                current = current.Get_Next();

            }
        }
        public void Remove_By_Key(int? key) 
        {
            if (key == head.Get_Data()) { Remove_From_Head(); return; }
            
            Node current = head;
            Node following = head.Get_Next();

            while (current.Get_Next() != null)
            {
                if (current.Get_Next().Get_Data() == key)
                {
                    current.Set_Next(following.Get_Next());
                    break;
                }

                following = following.Get_Next();
                current = current.Get_Next();
            }

        }
        public int? Max()
        {
            Node? current = head;
            int? max = current.Get_Data();

            while (current != null)
            {
                if (current.Get_Data() > max) max = current.Get_Data();
                current = current.Get_Next();
            }

            return max;

        }
        public int? Min()
        {
            Node? current = head;
            int? min = current.Get_Data();

            while (current != null)
            {
                if (current.Get_Data() < min) min = current.Get_Data();
                current = current.Get_Next();
            }

            return min;

        }
        public void Clear() { head = null; }
        public bool Empty_Check() {
            if (head == null || head.Get_Next() == null)
            {
                return true;
            }
            return false;
        }
        public int Size()
        {
            if (head == null) return 0;
            
            Node current = head;

            int size = 1;
            while (current.Get_Next() != null)
            {
                current = current.Get_Next();
                size++;
            }

            return size;
        }
        // operators
        public static bool operator == (List A, List B)
        {
            if (A.Size() != B.Size()) return false;

            Node? current1 = A.head;
            Node? current2 = B.head;
            while(current1 != null)
            {
                if (current1.Get_Data() != current2.Get_Data()) return false;
                current1 = current1.Get_Next();
                current2 = current2.Get_Next();
            }
            return true;
        }
        public static bool operator != (List A, List B)
        {
            return !(A == B);
        }
        public static List operator + (List A, List B) 
        {
            Node current = A.head;

            while (current.Get_Next() != null)
                current = current.Get_Next();

            current.Set_Next(B.head);
            return A;

        }
        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        public override bool Equals(object o)
        {
            if (o == null || !(o is List))
            {
                return false;
            }
            return this == (List)o;
        }

        public Node FindPos(int pos)
        {
            if (head == null)
            { Console.WriteLine("EmptyList"); return null; }

            int size = Size();

            if (size < pos)
            { Console.WriteLine("NotFound"); return null; }

            Node current = head;

            for (int i = 0; i < size; i++)
            {
                if (i == pos - 1)
                {
                    Console.WriteLine(current.Get_Data());
                    return current;
                }
                current = current.Get_Next();
            }
            return null;
        }
    }
}