using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace List_and_Set
{
    class Node
    {
        private int? data;  // value
        private Node? next; // link to next
        public Node() { data = null; 
            next = null; }  // constructor
        public Node(int? Data) { data = Data; } // constructor 2
        public int? Get_Data() { return data; } // getter
        public void Set_Data(int? Data) { data = Data; } // setter
        public Node? Get_Next() { return next; } // getter
        public void Set_Next(Node Next) { next = Next; } // setter
        public void print()
        {
            if(data != null) { Console.WriteLine(data); }
        }
    }
}
