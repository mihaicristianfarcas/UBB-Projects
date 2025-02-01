package Model;

import Model.Exceptions.EmptyExecutionStackException;
import Model.Statements.IStmt;
import Model.Utils.*;
import Model.Values.StringValue;
import Model.Values.Value;
import java.io.BufferedReader;
import java.io.IOException;

public class PrgState {
    MyIStack<IStmt> exeStack;
    MyIDictionary<String, Value> symTable;
    MyIList<Value> out;
    MyIDictionary<StringValue, BufferedReader> fileTable;
    MyIHeap<Value> heap;
    IStmt originalProgram;
    public static int id = 1;
    int currentID;
    private MyISemaphoreTable semaphoreTable;

    public PrgState(IStmt prog) {
        this.exeStack = new MyStack<>();
        this.symTable = new MyDictionary<>();
        this.fileTable = new MyDictionary<>();
        this.out = new MyList<>();
        this.heap = new MyHeap<>();
        this.originalProgram = prog.deepCopy();
        this.exeStack.push(prog);
        this.currentID = getNewID();
        this.semaphoreTable = new MySemaphoreTable();
    }

    public Integer getCurrentID() {
        return currentID;
    }

    private static synchronized int getNewID() {
        return id++;
    }

    public MyIStack<IStmt> getExeStack() {
        return exeStack;
    }

    public MyIDictionary<String, Value> getSymTable() {
        return symTable;
    }

    public MyIDictionary<StringValue, BufferedReader> getFileTable() {
        return fileTable;
    }

    public MyIList<Value> getOut() {
        return out;
    }

    public Boolean isNotCompleted() {
        return !exeStack.isEmpty();
    }

    public PrgState oneStep() throws EmptyExecutionStackException, IOException {
        if (exeStack.isEmpty())
            throw new EmptyExecutionStackException();
        IStmt currentStmt = exeStack.pop();
        return currentStmt.execute(this);
    }

    public IStmt getOriginalProgram() {
        return originalProgram;
    }

    public String toString() {
        return "-- ID --\n" + currentID + "\n-- ExeStack --\n" + exeStack.toString() + "\n-- SymTable --\n"
                + symTable.toString() + "-- Heap --\n" + heap.toString() + "-- Out --\n" + out.toString()
                + "\n-- FileTable --\n" + fileTable.toString() + "\n-- SemaphoreTable --\n" + semaphoreTable.toString()
                + "\n\n";
    }

    public MyIHeap<Value> getHeap() {
        return heap;
    }

    public void setSymTable(MyIDictionary<String, Value> clonedSymTable) {
        symTable = clonedSymTable;
    }

    public void setHeap(MyIHeap<Value> heap) {
        this.heap = heap;
    }

    public void setFileTable(MyIDictionary<StringValue, BufferedReader> fileTable) {
        this.fileTable = fileTable;
    }

    public void setOut(MyIList<Value> out) {
        this.out = out;
    }

    public MyISemaphoreTable getSemaphoreTable() {
        return semaphoreTable;
    }

    public void setSemaphoreTable(MyISemaphoreTable semaphoreTable) {
        this.semaphoreTable = semaphoreTable;
    }
}
