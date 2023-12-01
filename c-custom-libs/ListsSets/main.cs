
namespace List_and_Set
{
    class Program
    {
        static void Main()
        {
            Random rnd = new Random();
            

            Console.WriteLine("1)Создайте множество S1, из 10 случайных чисел. Выведите S1 на экран (используя функцию Print).");
            Set SS1= new Set();
            while(SS1.Get_Power() < 10)
            {
                SS1.Add_To_Set(rnd.Next(1, 30));
            }
            Console.Write("S1: ");
            SS1.Output_Set();
            Console.ReadLine();
            Console.WriteLine("2)Создайте множество S2 и инициализируйте его (при создании) значением S1.  " +
                "Выведите S2 на экран (используйте потоковый вывод). Проверьте равенство множеств S1  и  S2.");
            Set SS2 = SS1;
            Console.Write("S2: ");
            SS2.Output_Set();
            if (SS2 == SS1) { Console.WriteLine("S1 equals S2"); }
            else { Console.WriteLine("S1 not equals S2"); }
            Console.ReadLine();
            Console.WriteLine("3)\tПроверьте, есть ли в S1 элемент 5. Создайте множество S3, которое получается удалением/добавлением из S1 элемента 5. " +
                "Проверьте, что S1 и S3 – не равны.");
            Set SS3 = new Set(SS1);
            if (SS1.Find(5) != null)
            {
                Console.WriteLine("5 was found");
                SS3.Remove_From_Set(5);
            }
            else
            {
                Console.WriteLine("5 was not found");
                SS3.Add_To_Set(5);
            }
            Console.Write("S1: ");
            SS1.Output_Set();
            Console.Write("S3: ");
            SS3.Output_Set();
            if (SS3 == SS1) { Console.WriteLine("S1 equals S3"); }
            else { Console.WriteLine("S1 not equals S3"); }

            Console.ReadLine();
            Console.WriteLine("4)\tСоздайте пустое множество S4. Проверьте его на пустоту.  Добавьте в S4 последовательно числа 5, 10, 15, 5.  " +
                "Выведите S4 на экран.");
            Set SS4 = new Set();
            if(SS4.Empty_Check())
            {
                Console.WriteLine("Empty");
            }
            SS4.Add_To_Set(5);
            SS4.Add_To_Set(10);
            SS4.Add_To_Set(15);
            SS4.Add_To_Set(5);
            Console.Write("S4: ");
            SS4.Output_Set();
            Console.ReadLine();
            Console.WriteLine("5)\tСоздайте пустое множество S5.  Инициализируйте его множеством S4.  " +
                "Проверьте, что во множестве S5 есть элемент 15 и удалите его. Выведите получившееся множество на экран.");
            Set SS5 = new Set(SS4);
            SS5.Remove_From_Set(15);
            Console.Write("S5: ");
            SS5.Output_Set();
            Console.ReadLine();
            Console.WriteLine("6)\tСоздайте список T, из 20 случайных чисел. Выведите T на экран. " +
                "Создайте из T множество S6.  Выведите S6 на экран. Определите количество элементов в S6.");
            List T = new List();
            for(int i = 0; i < 20;i++)
            {
                T.Add_To_Tail(rnd.Next(1, 50));
            }
            Set SS6 = new Set(T);
            Console.Write("S6: ");
            SS6.Output_Set();
            Console.WriteLine(SS6.Size());
            Console.WriteLine("7)Найдите S7 – дополнение S6 до универсального. Найдите множество S8=S7ПS6.");
            Set SS7 = SS6.Complement();
            Console.Write("S7: ");
            SS7.Output_Set();
            Set SS8 = SS6.Intersection(SS7);
            Console.Write("S8: ");
            SS8.Output_Set();
            Console.ReadLine();
            Console.WriteLine("8)Создайте множество S9={1,3,5,7,9,11,13,15,17,19,21,23,25,27,29}(используйте потоковый ввод).  " +
                "Найдите V1 =S7 П S9,  V2 = S7 U S9,  V3 = S7 - S9.");
            Set SS9 = new Set();
            for(int i = 1;i < 30;i+=2)
            {
                SS9.Add_To_Set(i);
            }
            Console.Write("S9: ");
            SS9.Output_Set();
            Set V1 = SS7.Intersection(SS9);
            Console.Write("V1: ");
            V1.Output_Set();
            Set V2 = SS7.Unity(SS9);
            Console.Write("V2: ");
            V2.Output_Set();
            Set V3 = SS7.Difference(SS9);
            Console.Write("V3: ");
            V3.Output_Set();
            Console.ReadLine();
            Console.WriteLine("9)Измените V1, объединив его с V3. Сравните V1  с S7.\r\n");
            V1 = V1.Unity(V3);
            Console.Write("V1: ");
            V1.Output_Set();
            if(V1 == SS7) { Console.WriteLine("V1 equals S7"); }
            else
            {
                Console.WriteLine("V1 not equals S7");
            }
            Console.ReadLine();
            Console.WriteLine("10)Измените множество V2, заменив его разностью V2 и V3. Сравните V2  с  S9");
            V2 = V2.Difference(V3);
            Console.Write("V2: ");
            V2.Output_Set();
            if (V1 == SS7) { Console.WriteLine("V2 equals S9"); }
            else
            {
                Console.WriteLine("V2 not equals S9");
            }
        }
    }
}