///////////////////////////////////////////////////////////////
// Test.cs - startup multiple concurrent RequestProcessors   //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014 //
///////////////////////////////////////////////////////////////
/*
 *  What you need to add to be compliant:
 *  - Build a WPF application that calls into these functions
 *  - Compile this as a library and the WPF will add a reference
 *    to the library.  Don't use the __declspec qualifiers.
 *    That's a native C\C++ protocol.  You can just change
 *    the applcation property to library in the Project Properties.
 *  - You will need to make the class and functions public
 *  - You need to parse the information coming in from the
 *    RequestProcessors to provide meaningful results to the
 *    user via the WPF window.
 *  - That almost certainly implies changes to the way the
 *    Request Processors return their information.
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.IO;

namespace Tester
{
  class TestSpawn
  {
    object synch = new object();
    List<string> Results = new List<string>();
    string path = "..\\..\\..\\Debug";

    //----< return List of test results >------------------------------

    List<string> getResults() { return Results; }

    //----< is this the right directory? >-----------------------------

    void showFilesOnPath()
    {
      Console.Write("\n  {0}", Directory.GetCurrentDirectory());
      string[] files = Directory.GetFiles(path);
      foreach (string file in files)
        Console.Write("\n  {0}", file);
      Console.Write("\n");
    }
    //----< Run RequestProcessor showing its window >------------------

    void startTestInWindow()
    {
      ProcessStartInfo pInfo = new ProcessStartInfo();
      pInfo.FileName = path + "\\RequestProcessor.exe";
      pInfo.Arguments = "TextSearch 2 \"this is a search string\" dstMachine:127.0.0.1:8080 dstName:TextSearch";
      Process rpProcess = Process.Start(pInfo);
      rpProcess.WaitForExit();
      rpProcess.Dispose();
    }
    //----< Run RequestProcessor windowless >--------------------------

    void startTestNoWindow()
    {
      ProcessStartInfo pInfo = new ProcessStartInfo();
      pInfo.FileName = path + "\\RequestProcessor.exe";
      pInfo.Arguments = "TextSearch 2 \"this is a search string\" dstMachine:127.0.0.1:8080 dstName:TextSearch";
      pInfo.UseShellExecute = false;
      pInfo.CreateNoWindow = true;
      pInfo.RedirectStandardOutput = true;
      using (Process rpProcess = Process.Start(pInfo))
      {
        using (StreamReader rdr = rpProcess.StandardOutput)
        {
          while(true)
          {
            if (rdr.EndOfStream)
              break;
            string result = rdr.ReadToEnd();
            lock(synch)
            {
              Results.Add(result);
            }
            //Console.Write("\n  {0}", result);
          }
        }
      }
    }

    void processing(string[] args)
    {
      Console.Write("\n  Tester running RequestProcess");
      Console.Write("\n ===============================\n");

      //startTestInWindow();

      if(args.Count() == 0)
      {
        Console.Write("\n\n  Please enter on command line number of RequestProcessors to Start\n\n");
        return;
      }
      // run specified number of RequestProcessors

      int numToStart = Int32.Parse(args[0]);
      List<Task> taskList = new List<Task>();
      for (int i = 0; i < numToStart; ++i )
      {
        taskList.Add(Task.Run(() => startTestNoWindow()));
      }
      // make sure we have all the results

      foreach (Task task in taskList)
        task.Wait();
      
      // show the results

      List<string> Results = getResults();
      foreach (string result in Results)
        Console.Write(result);

      Console.Write("\n  Test complete\n");
    }
    //----< Run Tests >------------------------------------------------

    static void Main(string[] args)
    {
      TestSpawn ts = new TestSpawn();
      ts.processing(args);
    }
  }
}
