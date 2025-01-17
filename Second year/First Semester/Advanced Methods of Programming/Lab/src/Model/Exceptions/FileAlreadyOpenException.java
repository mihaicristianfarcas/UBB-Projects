package Model.Exceptions;

public class FileAlreadyOpenException extends FileException {
    public FileAlreadyOpenException(String message) {
        super(message);
    }
}
