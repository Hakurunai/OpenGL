using Eto.Forms;

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
        const DebugInfo debugInfo = DebugInfo.All;
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

            GLAD_Getter glad_Getter = new GLAD_Getter();
            Task GLAD = glad_Getter.LoadData(currentDir, libDir);

            GLFW_Getter glfw_Getter = new GLFW_Getter();
            Task GLFW = glfw_Getter.LoadData(currentDir, libDir);

            Task.WaitAll(GLAD, GLFW);

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