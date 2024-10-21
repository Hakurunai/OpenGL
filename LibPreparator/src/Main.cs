using Eto.Forms;
using System.Threading.Tasks;

namespace LPrep
{
#if DEBUG
    enum DebugInfo
    {
        All,
        Nothing
    }
#endif

    class Program
    {
#if DEBUG
        const DebugInfo debugInfo = DebugInfo.Nothing;
#endif

        static void Main(string[] args)
        {
            string currentDir = Environment.CurrentDirectory;
            string libDir = string.Empty;
           
            // Check if running in Visual Studio
            if (Environment.GetEnvironmentVariable("RUNNING_IN_VS") == "true")
                libDir = Path.GetFullPath(Path.Combine(currentDir, @"..\..\..\..\..\..\thirdParty\libs"));
            else
                libDir = Path.GetFullPath(Path.Combine(currentDir, @"..\..\..\thirdParty\libs"));
            Console.WriteLine($"Lib directory : {libDir}");

            GLFW_Getter glfw_Getter = new GLFW_Getter();
#if DEBUG
            if (debugInfo == DebugInfo.All)
            {
                glfw_Getter.LoadData(currentDir, libDir).Wait();
            }
            else
            { 
                Task GLFW = glfw_Getter.LoadData(currentDir, libDir);
                Task.WaitAll(GLFW);
            }
#else
            Task GLFW = glfw_Getter.LoadData(currentDir, libDir);
            Task.WaitAll(GLFW);
#endif

            {
                // Create an instance of CheckOpenGL
                Application app = new Application();
                var checkOpenGL = new CheckOpenGL(app);

                // Run the Eto.Forms application
                app.Run();
            }

            Console.WriteLine("Setup complete ! Press a key to exit.");
            //Console.ReadLine();
        }
    }
}