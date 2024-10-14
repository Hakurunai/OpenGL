namespace LPrep
{
    internal class GLAD_Getter : DataGetter
    {
        public override async Task LoadData(string CurrentDir, string LibDir)
        {
            {
                Console.WriteLine("Checking presence of glad files...");
                string gladFile = Path.Combine(LibDir, "Glad\\include\\glad\\glad.h");
                string khrFile = Path.Combine(LibDir, "Glad\\include\\KHR\\khrplatform.h");
                if (Path.Exists(gladFile) && Path.Exists(khrFile))
                {
                    Console.WriteLine("Glad's files are already there. Skipping download.");
                    return;
                }
            }

            // Hardcoded GLAD URL
            string gladZipUrl = "https://glad.dav1d.de/generated/tmp_dohdm05glad/glad.zip";
            string tempArchivePath = Path.Combine(CurrentDir, "glad.zip");
            string extractPath = Path.Combine(LibDir, "Glad");

            //clean any left over from previous work
            if(File.Exists(tempArchivePath))
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
                Console.WriteLine("Downloading GLAD archive...");
                await FileHelper.DownloadFileAsync(gladZipUrl, tempArchivePath);
                Console.WriteLine("Download complete.");

                Console.WriteLine("Extracting GLAD archive...");
                FileHelper.ExtractZipFile(tempArchivePath, extractPath);
                Console.WriteLine("Extraction complete.");

                Console.WriteLine("Cleaning temp files...");
                File.Delete(tempArchivePath);
                Console.WriteLine("Glad process completed successfully!");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"An error occurred: {ex.Message}");
                Console.ReadLine();
            }
        }
    }
}