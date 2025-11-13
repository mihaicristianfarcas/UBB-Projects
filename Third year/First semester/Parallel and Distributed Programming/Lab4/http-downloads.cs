using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace PPDLab4
{
    internal record DownloadRequest(string Host, string Path, string FileName, int Port = 80)
    {
        public string GetTargetPath(string baseFolder)
        {
            Directory.CreateDirectory(baseFolder);
            return Path.Combine(baseFolder, FileName);
        }
    }

    internal sealed class HttpDownloadSession : IDisposable
    {
        private readonly FileStream _output;
        private readonly List<byte> _headerBytes = new();
        private bool _headersComplete;
        private bool _connectionClosed;
        private int _bodyBytesWritten;
        private int? _contentLength;
        private bool _disposed;

        public HttpDownloadSession(DownloadRequest request, string targetPath)
        {
            Request = request;
            Directory.CreateDirectory(Path.GetDirectoryName(targetPath) ?? ".");
            _output = new FileStream(targetPath, FileMode.Create, FileAccess.Write, FileShare.None);
            TargetPath = targetPath;
        }

        public DownloadRequest Request { get; }
        public string TargetPath { get; }
        public IReadOnlyDictionary<string, string> Headers { get; private set; } =
            new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);

        public int BytesWritten => _bodyBytesWritten;
        public int? ExpectedBytes => _contentLength;
        public bool HeadersParsed => _headersComplete;
        public bool IsComplete =>
            (_contentLength.HasValue && _bodyBytesWritten >= _contentLength.Value) ||
            (!_contentLength.HasValue && _connectionClosed);

        public bool ReceivedAllExpectedBytes => !_contentLength.HasValue || _bodyBytesWritten >= _contentLength.Value;

        public void ProcessBytes(byte[] buffer, int bytesRead)
        {
            if (bytesRead <= 0)
            {
                return;
            }

            int offset = 0;
            if (!_headersComplete)
            {
                offset = ConsumeHeaders(buffer, bytesRead);
            }

            int bodyCount = bytesRead - offset;
            if (bodyCount > 0)
            {
                WriteBody(buffer, offset, bodyCount);
            }
        }

        private int ConsumeHeaders(byte[] buffer, int bytesRead)
        {
            int offset = 0;
            while (offset < bytesRead && !_headersComplete)
            {
                _headerBytes.Add(buffer[offset]);
                offset++;

                if (HeaderTerminatorDetected())
                {
                    _headersComplete = true;
                    ParseHeaders();
                    break;
                }
            }

            return offset;
        }

        private bool HeaderTerminatorDetected()
        {
            int count = _headerBytes.Count;
            if (count < 4)
            {
                return false;
            }

            return _headerBytes[count - 4] == '\r' &&
                   _headerBytes[count - 3] == '\n' &&
                   _headerBytes[count - 2] == '\r' &&
                   _headerBytes[count - 1] == '\n';
        }

        private void ParseHeaders()
        {
            var headerText = Encoding.ASCII.GetString(_headerBytes.ToArray());
            var lines = headerText.Split(new[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
            var headers = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);

            foreach (var line in lines.Skip(1)) // Skip status line
            {
                int colonIndex = line.IndexOf(':');
                if (colonIndex <= 0)
                {
                    continue;
                }

                string name = line[..colonIndex].Trim();
                string value = line[(colonIndex + 1)..].Trim();
                headers[name] = value;

                if (string.Equals(name, "Content-Length", StringComparison.OrdinalIgnoreCase) &&
                    int.TryParse(value, out var length))
                {
                    _contentLength = length;
                }
            }

            Headers = headers;
        }

        private void WriteBody(byte[] buffer, int offset, int count)
        {
            if (count <= 0)
            {
                return;
            }

            if (_contentLength.HasValue)
            {
                int remaining = _contentLength.Value - _bodyBytesWritten;
                if (remaining <= 0)
                {
                    return;
                }

                count = Math.Min(count, remaining);
            }

            _output.Write(buffer, offset, count);
            _bodyBytesWritten += count;
        }

        public void MarkConnectionClosed()
        {
            _connectionClosed = true;
        }

        public void Dispose()
        {
            if (_disposed)
            {
                return;
            }

            _output.Dispose();
            _disposed = true;
        }
    }

    internal readonly record struct DownloadResult(string Implementation, string FilePath, int BytesWritten, int? ExpectedBytes);

    internal interface IHttpDownloader
    {
        string Name { get; }

        Task<DownloadResult> DownloadAsync(DownloadRequest request, string baseFolder);
    }

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

    internal static class SocketTaskExtensions
    {
        public static Task ConnectAsync(Socket socket, string host, int port)
        {
            var tcs = new TaskCompletionSource<bool>(TaskCreationOptions.RunContinuationsAsynchronously);
            socket.BeginConnect(host, port, ar =>
            {
                try
                {
                    socket.EndConnect(ar);
                    tcs.TrySetResult(true);
                }
                catch (Exception ex)
                {
                    tcs.TrySetException(ex);
                }
            }, null);
            return tcs.Task;
        }

        public static Task<int> SendAsync(Socket socket, byte[] buffer, int offset, int count)
        {
            var tcs = new TaskCompletionSource<int>(TaskCreationOptions.RunContinuationsAsynchronously);
            socket.BeginSend(buffer, offset, count, SocketFlags.None, ar =>
            {
                try
                {
                    int sent = socket.EndSend(ar);
                    tcs.TrySetResult(sent);
                }
                catch (Exception ex)
                {
                    tcs.TrySetException(ex);
                }
            }, null);
            return tcs.Task;
        }

        public static Task<int> ReceiveAsync(Socket socket, byte[] buffer)
        {
            var tcs = new TaskCompletionSource<int>(TaskCreationOptions.RunContinuationsAsynchronously);
            socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, ar =>
            {
                try
                {
                    int read = socket.EndReceive(ar);
                    tcs.TrySetResult(read);
                }
                catch (Exception ex)
                {
                    tcs.TrySetException(ex);
                }
            }, null);
            return tcs.Task;
        }
    }

    internal static class SocketFactory
    {
        public static Socket Create()
        {
            var socket = new Socket(AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp)
            {
                DualMode = true
            };
            return socket;
        }
    }

    internal sealed class CallbackHttpDownloader : IHttpDownloader
    {
        public string Name => "Callbacks";

        public Task<DownloadResult> DownloadAsync(DownloadRequest request, string baseFolder)
        {
            var tcs = new TaskCompletionSource<DownloadResult>(TaskCreationOptions.RunContinuationsAsynchronously);
            var socket = SocketFactory.Create();
            var session = new HttpDownloadSession(request, request.GetTargetPath(baseFolder));
            var state = new CallbackState(socket, session, request, tcs);

            try
            {
                socket.BeginConnect(request.Host, request.Port, OnConnected, state);
            }
            catch (Exception ex)
            {
                state.Fail(ex);
            }

            return tcs.Task;
        }

        private static void OnConnected(IAsyncResult ar)
        {
            if (ar.AsyncState is not CallbackState state)
            {
                return;
            }

            try
            {
                state.Socket.EndConnect(ar);
                BeginSend(state);
            }
            catch (Exception ex)
            {
                state.Fail(ex);
            }
        }

        private static void BeginSend(CallbackState state)
        {
            if (state.BytesSent >= state.RequestBytes.Length)
            {
                BeginReceive(state);
                return;
            }

            int remaining = state.RequestBytes.Length - state.BytesSent;
            state.Socket.BeginSend(state.RequestBytes, state.BytesSent, remaining, SocketFlags.None, OnSendCompleted, state);
        }

        private static void OnSendCompleted(IAsyncResult ar)
        {
            if (ar.AsyncState is not CallbackState state)
            {
                return;
            }

            try
            {
                int sent = state.Socket.EndSend(ar);
                state.BytesSent += sent;
                BeginSend(state);
            }
            catch (Exception ex)
            {
                state.Fail(ex);
            }
        }

        private static void BeginReceive(CallbackState state)
        {
            state.Socket.BeginReceive(state.Buffer, 0, state.Buffer.Length, SocketFlags.None, OnReceiveCompleted, state);
        }

        private static void OnReceiveCompleted(IAsyncResult ar)
        {
            if (ar.AsyncState is not CallbackState state)
            {
                return;
            }

            try
            {
                int bytes = state.Socket.EndReceive(ar);
                if (bytes <= 0)
                {
                    state.Session.MarkConnectionClosed();
                    if (state.Session.IsComplete && state.Session.HeadersParsed)
                    {
                        state.Success();
                    }
                    else
                    {
                        state.Fail(new IOException("Connection closed before Content-Length bytes were received."));
                    }

                    return;
                }

                state.Session.ProcessBytes(state.Buffer, bytes);
                if (state.Session.IsComplete)
                {
                    state.Success();
                }
                else
                {
                    BeginReceive(state);
                }
            }
            catch (Exception ex)
            {
                state.Fail(ex);
            }
        }

        private sealed class CallbackState : IDisposable
        {
            public CallbackState(Socket socket, HttpDownloadSession session, DownloadRequest request, TaskCompletionSource<DownloadResult> tcs)
            {
                Socket = socket;
                Session = session;
                Request = request;
                CompletionSource = tcs;
                RequestBytes = HttpRequestBuilder.BuildRequestBytes(request);
            }

            public Socket Socket { get; }
            public HttpDownloadSession Session { get; }
            public DownloadRequest Request { get; }
            public TaskCompletionSource<DownloadResult> CompletionSource { get; }
            public byte[] Buffer { get; } = new byte[8192];
            public byte[] RequestBytes { get; }
            public int BytesSent { get; set; }
            public bool Completed { get; private set; }

            public void Success()
            {
                if (Completed)
                {
                    return;
                }

                Completed = true;
                var result = new DownloadResult("Callbacks", Session.TargetPath, Session.BytesWritten, Session.ExpectedBytes);
                Cleanup();
                CompletionSource.TrySetResult(result);
            }

            public void Fail(Exception ex)
            {
                if (Completed)
                {
                    return;
                }

                Completed = true;
                Cleanup();
                CompletionSource.TrySetException(ex);
            }

            private void Cleanup()
            {
                Dispose();
            }

            public void Dispose()
            {
                Session.Dispose();
                try
                {
                    if (Socket.Connected)
                    {
                        Socket.Shutdown(SocketShutdown.Both);
                    }
                }
                catch
                {
                    // Ignore shutdown problems during cleanup.
                }

                Socket.Dispose();
            }
        }
    }

    internal sealed class ContinuationHttpDownloader : IHttpDownloader
    {
        public string Name => "Continuations";

        public Task<DownloadResult> DownloadAsync(DownloadRequest request, string baseFolder)
        {
            var tcs = new TaskCompletionSource<DownloadResult>(TaskCreationOptions.RunContinuationsAsynchronously);
            var socket = SocketFactory.Create();
            var session = new HttpDownloadSession(request, request.GetTargetPath(baseFolder));
            var buffer = new byte[8192];
            var requestBytes = HttpRequestBuilder.BuildRequestBytes(request);

            SocketTaskExtensions.ConnectAsync(socket, request.Host, request.Port).ContinueWith(connectTask =>
            {
                if (connectTask.IsFaulted)
                {
                    FinishWithFailure(connectTask.Exception?.InnerException ?? connectTask.Exception!, socket, session, tcs);
                    return;
                }

                SendRequest(socket, requestBytes, 0, () => StartReceiving(socket, buffer, session, tcs), ex => FinishWithFailure(ex, socket, session, tcs));
            }, TaskContinuationOptions.ExecuteSynchronously);

            return tcs.Task;
        }

        private static void SendRequest(Socket socket, byte[] payload, int offset, Action onCompleted, Action<Exception> onError)
        {
            if (offset >= payload.Length)
            {
                onCompleted();
                return;
            }

            SocketTaskExtensions.SendAsync(socket, payload, offset, payload.Length - offset).ContinueWith(sendTask =>
            {
                if (sendTask.IsFaulted)
                {
                    onError(sendTask.Exception?.InnerException ?? sendTask.Exception!);
                    return;
                }

                int sent = sendTask.Result;
                SendRequest(socket, payload, offset + sent, onCompleted, onError);
            }, TaskContinuationOptions.ExecuteSynchronously);
        }

        private static void StartReceiving(Socket socket, byte[] buffer, HttpDownloadSession session, TaskCompletionSource<DownloadResult> tcs)
        {
            SocketTaskExtensions.ReceiveAsync(socket, buffer).ContinueWith(receiveTask =>
            {
                if (receiveTask.IsFaulted)
                {
                    FinishWithFailure(receiveTask.Exception?.InnerException ?? receiveTask.Exception!, socket, session, tcs);
                    return;
                }

                int bytesRead = receiveTask.Result;
                if (bytesRead <= 0)
                {
                    session.MarkConnectionClosed();
                    if (session.IsComplete && session.HeadersParsed)
                    {
                        FinishWithSuccess(socket, session, tcs);
                    }
                    else
                    {
                        FinishWithFailure(new IOException("Connection closed before Content-Length bytes were received."), socket, session, tcs);
                    }

                    return;
                }

                session.ProcessBytes(buffer, bytesRead);
                if (session.IsComplete)
                {
                    FinishWithSuccess(socket, session, tcs);
                }
                else
                {
                    StartReceiving(socket, buffer, session, tcs);
                }
            }, TaskContinuationOptions.ExecuteSynchronously);
        }

        private static void FinishWithSuccess(Socket socket, HttpDownloadSession session, TaskCompletionSource<DownloadResult> tcs)
        {
            var result = new DownloadResult("Continuations", session.TargetPath, session.BytesWritten, session.ExpectedBytes);
            Cleanup(socket, session);
            tcs.TrySetResult(result);
        }

        private static void FinishWithFailure(Exception exception, Socket socket, HttpDownloadSession session, TaskCompletionSource<DownloadResult> tcs)
        {
            Cleanup(socket, session);
            tcs.TrySetException(exception);
        }

        private static void Cleanup(Socket socket, HttpDownloadSession session)
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

    public static class Program
    {
        public static async Task Main()
        {
            var requests = new[]
            {
                new DownloadRequest("info.cern.ch", "/", "cern.html"),
                new DownloadRequest("neverssl.com", "/", "neverssl.html"),
                new DownloadRequest("placekitten.com", "/200/300", "kitten.jpg")
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
                var tasks = requests.Select(request => downloader.DownloadAsync(request, Path.Combine("Downloads", downloader.Name))).ToArray();
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
