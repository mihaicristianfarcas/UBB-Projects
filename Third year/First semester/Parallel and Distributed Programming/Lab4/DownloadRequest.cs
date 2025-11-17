using System.IO;

namespace PPDLab4
{
    internal record DownloadRequest(string Host, string Path, string FileName, int Port = 80)
    {
        public string GetTargetPath(string baseFolder)
        {
            Directory.CreateDirectory(baseFolder);
            return System.IO.Path.Combine(baseFolder, FileName);
        }
    }
}
