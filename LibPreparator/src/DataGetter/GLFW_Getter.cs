using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LPrep
{
    internal class GLFW_Getter : DataGetter
    {
        public override async Task LoadData(string CurrentDir, string LibDir)
        {
            {
                Console.WriteLine("Checking presence of glfw files...");
                bool isGlfwAlreadyDownloaded = true;
                List<string> necessaryFileList = new List<string>(3);
                necessaryFileList.Add(Path.Combine(LibDir, "glfw-3.4.bin.WIN64\\include\\GLFW\\glfw3.h"));
                necessaryFileList.Add(Path.Combine(LibDir, "glfw-3.4.bin.WIN64\\include\\GLFW\\glfw3native.h"));
                necessaryFileList.Add(Path.Combine(LibDir, "glfw-3.4.bin.WIN64\\lib-vc2022\\glfw3.lib"));

                foreach (var file in necessaryFileList)
                {
                    if (File.Exists(file))
                        continue;

                    Console.WriteLine($"At least {file} is missing.");
                    isGlfwAlreadyDownloaded = false;
                    break;
                }
                if (isGlfwAlreadyDownloaded)
                {
                    Console.WriteLine("GLFW's files are already there. Skipping download.");
                    return;
                }
            }

            string glfwUrl = "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip";
            string tempArchivePath = Path.Combine(CurrentDir, "glfw.zip");
            string extractPath = Path.Combine(LibDir, "glfw-3.4.bin.WIN64");

            //clean any left over from previous work
            if (File.Exists(tempArchivePath))
                File.Delete(tempArchivePath);

            if (Directory.Exists(extractPath))
                Directory.Delete(extractPath, true);

#if DEBUG
            debugInfo = DebugInfo.Nothing;
            if (debugInfo == DebugInfo.All)
            {
                Console.WriteLine($"Archive path : {tempArchivePath}");
                Console.WriteLine($"Extract path : {extractPath}");
                Console.WriteLine("Press Enter to continue...");
                Console.ReadLine();
            }
#endif
            try
            {
                Console.WriteLine("Downloading GLFW archive...");
                await FileHelper.DownloadFileAsync(glfwUrl, tempArchivePath);
                Console.WriteLine("Download complete.");

                Console.WriteLine("Extracting GLFW archive...");
                FileHelper.ExtractZipFile(tempArchivePath, extractPath);
                Console.WriteLine("Extraction complete.");

                Console.WriteLine("Moving files...");
                string newDirectoryName = "toDelete";
                string newPath = Path.Combine(Path.GetDirectoryName(extractPath)!, newDirectoryName);
                FileHelper.MoveDirectory(extractPath, newPath);

                string dir = Directory.GetDirectories(newPath)[0];
                string dest = Path.Combine(Directory.GetParent(newPath)!.FullName, Path.GetFileName(dir)!);
                FileHelper.MoveDirectory(dir, dest);

                Console.WriteLine("Cleaning temp files...");
                File.Delete(tempArchivePath);
                Directory.Delete(newPath, true);
                Console.WriteLine("GLFW process completed successfully!");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"An error occurred: {ex.Message}");
                Console.ReadLine();
            }
        }
    }
}