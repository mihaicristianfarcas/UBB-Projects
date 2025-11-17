using System;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace PPDLab4
{
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
}
