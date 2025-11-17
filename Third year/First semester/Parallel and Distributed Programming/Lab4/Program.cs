using System;
using System.Linq;
using System.Threading.Tasks;

namespace PPDLab4
{
    public static class Program
    {
        public static async Task Main()
        {
            var requests = new[]
            {
                new DownloadRequest("info.cern.ch", "/", "cern.html"),
                new DownloadRequest("neverssl.com", "/", "neverssl.html"),
                new DownloadRequest("example.com", "/", "example.html")
            };

            var downloaders = new IHttpDownloader[]
            {
                new CallbackHttpDownloader(),
                new ContinuationHttpDownloader(),
                new AsyncAwaitHttpDownloader()
            };

            foreach (var downloader in downloaders)
            {
                Console.WriteLine($"Running {downloader.Name} implementation...");
                var tasks = requests.Select(request => downloader.DownloadAsync(request, System.IO.Path.Combine("Downloads", downloader.Name))).ToArray();
                var results = await Task.WhenAll(tasks);

                foreach (var result in results)
                {
                    Console.WriteLine($"[{result.Implementation}] {result.FilePath} -> {result.BytesWritten} bytes (expected: {result.ExpectedBytes?.ToString() ?? "unknown"})");
                }

                Console.WriteLine();
            }

            Console.WriteLine("All downloads completed.");
        }
    }
}
