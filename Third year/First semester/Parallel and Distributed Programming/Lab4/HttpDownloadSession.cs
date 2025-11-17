using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace PPDLab4
{
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
            Directory.CreateDirectory(System.IO.Path.GetDirectoryName(targetPath) ?? ".");
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

            foreach (var line in lines.Skip(1))
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
}
