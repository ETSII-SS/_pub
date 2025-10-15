using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication1
{
    class Program
    {
        static byte[] Matriz;
        static void Main(string[] args)
        {
            Matriz = new byte[1024 * 1024 * 1024];
            Console.WriteLine("Creada la matriz");
            for (int i = 0; i < Matriz.Length; i++)
            {
                //for (int j = 0; j < 10; j++) ;
                Matriz[i] = 10;
            }
            Matriz = null; //Liberada (aunque no inmediato)
            GC.Collect();  // Equivalente a System.gc();

            Console.WriteLine("Liberada la matriz");
         
        }
    }
}
