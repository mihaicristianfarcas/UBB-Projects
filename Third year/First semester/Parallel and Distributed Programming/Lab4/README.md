# HTTP Downloads Lab (TPL + Sockets)

## What this project does
- Starts three separate download strategies (callbacks, continuations, async/await) that all reuse the same HTTP parser and buffer management.
- Each strategy fires off three HTTP GETs in parallel so the files are fetched concurrently.
- Networking is implemented directly with `BeginConnect`/`EndConnect`, `BeginSend`/`EndSend`, and `BeginReceive`/`EndReceive` on a raw `Socket`, exactly as required in the lab statement.
- A minimal HTTP parser reads header lines from the byte stream, detects the double CRLF terminator, and understands the `Content-Length` header so the download code knows when the body is complete even if the remote endpoint keeps the TCP connection open.

## Mapping to the lab requirements
| Requirement | How it is satisfied |
| --- | --- |
| *Use C# TPL futures/continuations together with external events* | The callback-first version creates a `TaskCompletionSource` future that is completed from the socket callbacks; the `ContinuationHttpDownloader` chains the custom socket tasks with `ContinueWith`; the `AsyncAwaitHttpDownloader` builds on the same wrappers and relies on the TPL scheduler for continuations behind `await`. |
| *Call `BeginConnect`/`BeginSend`/`BeginReceive` directly* | The callback implementation invokes these methods explicitly; the other two variants call the same methods through thin wrappers where the callbacks set `TaskCompletionSource` results. |
| *Write a simple HTTP parser that understands headers and `Content-Length`* | `HttpDownloadSession` gathers bytes until it sees `\r\n\r\n`, splits the header lines, stores them case-insensitively, and parses `Content-Length` so that body reads stop exactly at that boundary. |
| *Simultaneously download several files* | `Program.Main` builds the request list and, for each implementation, starts all downloads and awaits `Task.WhenAll`, ensuring the transfers run concurrently without `Wait()` or blocking calls. |
| *No `Wait()` outside `Main`* | The source contains no `.Wait()` invocations. `Main` itself is `async Task` and uses `await Task.WhenAll`, which adheres to the note in the lab description. |

## Running the sample
You can execute the single-file program with the .NET SDK installed:

```bash
dotnet script http-downloads.cs
# or compile it:
csc /t:exe /out:HttpDownloads.exe http-downloads.cs && ./HttpDownloads
```

> The program saves the downloaded files under `Downloads/<ImplementationName>/`. Delete that folder between runs if you want a clean slate.

## How each implementation works
- **Callbacks** – pure event-driven flow where every socket callback (`OnConnected`, `OnSendCompleted`, `OnReceiveCompleted`) drives the next operation and completes a `TaskCompletionSource` when the transfer finishes.
- **Continuations** – wraps the socket operations in tasks, then uses `ContinueWith` to build the same state machine as a series of continuations (connect → send loop → receive loop).
- **AsyncAwait** – reuses the task-based socket wrappers but expresses the control flow with `await`, keeping the logic linear while still relying on the asynchronous socket APIs.

All three variants share the same `HttpDownloadSession`, ensuring identical parsing, file writing, and completion checks regardless of the control-flow mechanism.
