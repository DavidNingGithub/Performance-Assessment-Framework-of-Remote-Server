using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CLRclient;
namespace use
{
    class Program
    {
        static void Main(string[] args)
        {
            ManagedClient user = new ManagedClient();
            user.start(8082, 1, 0, 2, "test test test", "8082");

        }
    }
}
