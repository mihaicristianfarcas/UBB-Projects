using System.Text;

namespace PPDLab4
{
    internal static class HttpRequestBuilder
    {
        public static byte[] BuildRequestBytes(DownloadRequest request)
        {
            string text = $"GET {request.Path} HTTP/1.1\r\n" +
                          $"Host: {request.Host}\r\n" +
                          "Connection: close\r\n\r\n";
            return Encoding.ASCII.GetBytes(text);
        }
    }
}
