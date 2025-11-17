using System;
using System.IO;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace PPDLab4
{
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
}
