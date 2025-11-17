using System.Net.Sockets;

namespace PPDLab4
{
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
}
