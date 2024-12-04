package Model;

import Model.Exceptions.EmptyExecutionStackException;
import Model.Statements.IStmt;
import Model.Utils.*;
import Model.Values.RefValue;
import Model.Values.StringValue;
import Model.Values.Value;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class PrgState {
    MyIStack<IStmt> exeStack;
    MyIDictionary<String, Value> symTable;
    MyIList<Value> out;
    MyIDictionary<StringValue, BufferedReader> fileTable;
    MyIHeap<Value> heap;
    IStmt originalProgram;
    static int id = 1;

    public PrgState(IStmt prog) {
        this.exeStack = new MyStack<>();
        this.symTable = new MyDictionary<>();
        this.fileTable = new MyDictionary<>();
        this.out = new MyList<>();
        this.heap = new MyHeap<>();
        originalProgram = prog.deepCopy();
        this.exeStack.push(prog);
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
        return exeStack.isEmpty();
    }

    public PrgState oneStep() throws EmptyExecutionStackException, IOException {
        if(exeStack.isEmpty()) throw new EmptyExecutionStackException();
        IStmt currentStmt = exeStack.pop();
        return currentStmt.execute(this);
    }

    public IStmt getOriginalProgram() {
        return originalProgram;
    }

    public String toString() {
        return "ID:\n" + id + "\nExeStack:\n" + exeStack.toString() + "\nSymTable:\n" + symTable.toString() + "\nHeap:\n" + heap.toString() + "\nOut:\n" + out.toString() + "\nFileTable:\n" + fileTable.toString() + "\n\n";
    }

    public MyIHeap<Value> getHeap() {
        return heap;
    }
}
