using System.IO;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace PPDLab4
{
    internal sealed class AsyncAwaitHttpDownloader : IHttpDownloader
    {
        public string Name => "AsyncAwait";

        public async Task<DownloadResult> DownloadAsync(DownloadRequest request, string baseFolder)
        {
            var socket = SocketFactory.Create();
            var session = new HttpDownloadSession(request, request.GetTargetPath(baseFolder));
            var buffer = new byte[8192];

            try
            {
                await SocketTaskExtensions.ConnectAsync(socket, request.Host, request.Port);

                var payload = HttpRequestBuilder.BuildRequestBytes(request);
                int offset = 0;
                while (offset < payload.Length)
                {
                    int sent = await SocketTaskExtensions.SendAsync(socket, payload, offset, payload.Length - offset);
                    offset += sent;
                }

                while (true)
                {
                    int bytesRead = await SocketTaskExtensions.ReceiveAsync(socket, buffer);
                    if (bytesRead <= 0)
                    {
                        session.MarkConnectionClosed();
                        break;
                    }

                    session.ProcessBytes(buffer, bytesRead);
                    if (session.IsComplete)
                    {
                        break;
                    }
                }

                if (!session.IsComplete || !session.HeadersParsed)
                {
                    throw new IOException("Connection closed before Content-Length bytes were received.");
                }

                return new DownloadResult("AsyncAwait", session.TargetPath, session.BytesWritten, session.ExpectedBytes);
            }
            finally
            {
                session.Dispose();
                try
                {
                    if (socket.Connected)
                    {
                        socket.Shutdown(SocketShutdown.Both);
                    }
                }
                catch
                {
                    // Ignore cleanup issues.
                }

                socket.Dispose();
            }
        }
    }
}
