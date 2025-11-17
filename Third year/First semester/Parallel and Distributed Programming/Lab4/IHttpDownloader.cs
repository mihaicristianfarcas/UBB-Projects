using System.Threading.Tasks;

namespace PPDLab4
{
    internal interface IHttpDownloader
    {
        string Name { get; }
        Task<DownloadResult> DownloadAsync(DownloadRequest request, string baseFolder);
    }
}
