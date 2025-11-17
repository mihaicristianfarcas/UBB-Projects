using System;
using System.IO;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace PPDLab4
{
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
}
