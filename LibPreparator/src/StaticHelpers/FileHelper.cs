using System.IO.Compression;

namespace LPrep
{
    internal static class FileHelper
    {
        static public async Task DownloadFileAsync(string url, string savePath)
        {
            using (HttpClient client = new HttpClient())
            {
                var response = await client.GetAsync(url);
                response.EnsureSuccessStatusCode();
                using (var fs = new FileStream(savePath, FileMode.CreateNew))
                {
                    await response.Content.CopyToAsync(fs);
                }
            }
        }

        static public void ExtractZipFile(string zipPath, string extractPath)
        {
            if (File.Exists(zipPath))
                ZipFile.ExtractToDirectory(zipPath, extractPath);
            else
                throw new FileNotFoundException($"{zipPath} does not exist.");
        }

        static public void MoveDirectory(string Source, string Destination, bool IsOriginDirectoryDeleted = true)
        {
            if (!Directory.Exists(Source))
                throw new FileNotFoundException($"{Source} is missing.");
            try
            {
                Directory.Move(Source, Destination);

                if (IsOriginDirectoryDeleted)
                    Directory.Delete(Source, true);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"An error occurred: {ex.Message}");
            }
        }

        static public void CopyFiles(string sourceDir, string targetDir)
        {
            foreach (string dirPath in Directory.GetDirectories(
                sourceDir, "*", SearchOption.AllDirectories))
            {
                Directory.CreateDirectory(dirPath.Replace(sourceDir, targetDir));
            }

            foreach (string newPath in Directory.GetFiles(
                sourceDir, "*.*", SearchOption.AllDirectories))
            {
                File.Copy(newPath, newPath.Replace(sourceDir, targetDir), true);
            }
        }
    }
}