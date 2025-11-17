namespace PPDLab4
{
    internal readonly record struct DownloadResult(string Implementation, string FilePath, int BytesWritten, int? ExpectedBytes);
}
