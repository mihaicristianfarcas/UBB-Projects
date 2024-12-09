package View;

import Controller.MyController;
import Model.Expressions.*;
import Model.PrgState;
import Model.Statements.*;
import Model.Types.BoolType;
import Model.Types.IntType;
import Model.Types.RefType;
import Model.Types.StringType;
import Model.Values.BoolValue;
import Model.Values.IntValue;
import Model.Values.StringValue;
import Repository.MyFileRepository;
import Repository.MyIRepository;


public class Interpreter {
    public static void main(String[] args) {

        // int v;
        // print(v);
        IStmt ex1 = new CompStmt(new VarDeclStmt("v", new IntType()), new PrintStmt(new VarExp("v")));
        PrgState prgState1 = new PrgState(ex1);
        MyIRepository repository1 = new MyFileRepository("log1.txt");
        repository1.addProgram(prgState1);
        MyController controller1 = new MyController(repository1);


        // int a;
        // int b;
        // a = 2 + 3 * 5;
        // b = a + 1;
        // print(b);
        IStmt ex2 = new CompStmt(
                new VarDeclStmt("a",new IntType()),
                new CompStmt(
                        new VarDeclStmt("b", new IntType()),
                        new CompStmt(
                                new AssignStmt("a", new ArithExp('+', new ValueExp(new IntValue(2)),
                                        new ArithExp('*', new ValueExp(new IntValue(3)), new ValueExp(new IntValue(5))))),
                                new CompStmt(
                                        new AssignStmt("b",new ArithExp('+',new VarExp("a"), new ValueExp(new IntValue(1)))),
                                        new PrintStmt(new VarExp("b"))))));
        PrgState prgState2 = new PrgState(ex2);
        MyIRepository repository2 = new MyFileRepository("log2.txt");
        repository2.addProgram(prgState2);
        MyController controller2 = new MyController(repository2);


        // bool a;
        // int v;
        // a=true;
        // (If a Then v=2 Else v=3);
        // Print(v);
        IStmt ex3 = new CompStmt(new VarDeclStmt("a",new BoolType()),
                    new CompStmt(new VarDeclStmt("v", new IntType()),
                        new CompStmt(new AssignStmt("a", new ValueExp(new BoolValue(true))),
                                new CompStmt(new IfStmt(new VarExp("a"),new AssignStmt("v",new ValueExp(new
                                        IntValue(2))), new AssignStmt("v", new ValueExp(new IntValue(3)))), new PrintStmt(new
                                        VarExp("v"))))));
        PrgState prgState3 = new PrgState(ex3);
        MyIRepository repository3 = new MyFileRepository("log3.txt");
        repository3.addProgram(prgState3);
        MyController controller3 = new MyController(repository3);

//        string varf;
//        varf="test.in";
//        openRFile(varf);
//        int varc;
//        readFile(varf,varc);
//        print(varc);
//        readFile(varf,varc);
//        print(varc);
//        closeRFile(varf);
        IStmt ex4 = new CompStmt(
                new VarDeclStmt("varf", new StringType()),
                new CompStmt(
                        new AssignStmt("varf", new ValueExp(new StringValue("test.in"))),
                        new CompStmt(
                                new OpenRFileStmt(new VarExp("varf")),
                                new CompStmt(
                                        new VarDeclStmt("varc", new IntType()),
                                        new CompStmt(
                                                new ReadFileStmt(new VarExp("varf"), "varc"),
                                                new CompStmt(
                                                        new PrintStmt(new VarExp("varc")),
                                                        new CompStmt(
                                                                new ReadFileStmt(new VarExp("varf"), "varc"),
                                                                new CompStmt(
                                                                        new PrintStmt(new VarExp("varc")),
                                                                        new CloseRFileStmt(new VarExp("varf"))
                                                                )
                                                        )
                                                )
                                        )
                                )
                        )
                )
        );
        PrgState prgState4 = new PrgState(ex4);
        MyIRepository repository4 = new MyFileRepository("log4.txt");
        repository4.addProgram(prgState4);
        MyController controller4 = new MyController(repository4);


//      Ref int v;
//      new(v, 20);
//      Ref Ref int a;
//      new(a, v);
//      print(v);
//      print(a);
        IStmt ex5 = new CompStmt(
                    new VarDeclStmt("v", new RefType(new IntType())),
                    new CompStmt(
                        new NewStmt("v", new ValueExp(new IntValue(20))),
                        new CompStmt(
                                new VarDeclStmt("a", new RefType(new RefType(new IntType()))),
                                new CompStmt(
                                        new NewStmt("a", new VarExp("v")),
                                        new CompStmt(
                                                new PrintStmt(new VarExp("v")),
                                                new PrintStmt(new VarExp("a"))
                                        )
                                )
                        )
                )
        );
        MyIRepository repository5 = new MyFileRepository("log5.txt");
        PrgState prgState5 = new PrgState(ex5);
        repository5.addProgram(prgState5);
        MyController controller5 = new MyController(repository5);

//      Ref int v;
//      new(v, 20);
//      Ref Ref int a;
//      new(a, v);
//      print(rH(v));
//      print(rH(rH(a))+5);
        IStmt ex6 = new CompStmt(
                new VarDeclStmt("v", new RefType(new IntType())),
                new CompStmt(
                        new NewStmt("v", new ValueExp(new IntValue(20))),
                        new CompStmt(
                                new VarDeclStmt("a", new RefType(new RefType(new IntType()))),
                                new CompStmt(
                                        new NewStmt("a", new VarExp("v")),
                                        new CompStmt(
                                                new PrintStmt(new ReadHeapExp(new VarExp("v"))),
                                                new PrintStmt(
                                                        new ArithExp(
                                                                '+',
                                                                new ReadHeapExp(new ReadHeapExp(new VarExp("a"))),
                                                                new ValueExp(new IntValue(5))
                                                        )
                                                )
                                        )
                                )
                        )
                )
        );
        MyIRepository repository6 = new MyFileRepository("log6.txt");
        PrgState prgState6 = new PrgState(ex6);
        repository6.addProgram(prgState6);
        MyController controller6 = new MyController(repository6);


//      Ref int v;
//      new(v, 20);
//      print(rH(v));
//      wH(v, 30);
//      print(rH(v)+5);
        IStmt ex7 = new CompStmt(
                new VarDeclStmt("v", new RefType(new IntType())),
                new CompStmt(
                        new NewStmt("v", new ValueExp(new IntValue(20))),
                        new CompStmt(
                                new PrintStmt(new ReadHeapExp(new VarExp("v"))),
                                new CompStmt(
                                        new WriteHeapStmt("v", new ValueExp(new IntValue(30))),
                                        new PrintStmt(
                                                new ArithExp(
                                                        '+',
                                                        new ReadHeapExp(new VarExp("v")),
                                                        new ValueExp(new IntValue(5))
                                                )
                                        )
                                )
                        )
                )
        );
        MyIRepository repository7 = new MyFileRepository("log7.txt");
        PrgState prgState7 = new PrgState(ex7);
        repository7.addProgram(prgState7);
        MyController controller7 = new MyController(repository7);

//      Ref int v;
//      new(v, 20);
//      Ref Ref int a;
//      new(a, v);
//      new(v, 30);
//      print(rH(rH(a)));
        IStmt ex8 = new CompStmt(
                new VarDeclStmt("v", new RefType(new IntType())),
                new CompStmt(
                        new NewStmt("v", new ValueExp(new IntValue(20))),
                        new CompStmt(
                                new VarDeclStmt("a", new RefType(new RefType(new IntType()))),
                                new CompStmt(
                                        new NewStmt("a", new VarExp("v")),
                                        new CompStmt(
                                                new NewStmt("v", new ValueExp(new IntValue(30))),
                                                new PrintStmt(
                                                        new ReadHeapExp(
                                                                new ReadHeapExp(new VarExp("a"))
                                                        )
                                                )
                                        )
                                )
                        )
                )
        );
        MyIRepository repository8 = new MyFileRepository("log8.txt");
        PrgState prgState8 = new PrgState(ex8);
        repository8.addProgram(prgState8);
        MyController controller8 = new MyController(repository8);

//      int v;
//      v = 4;
//      (
//       while(v > 0)
//       print(v);
//       v = v - 1
//      );
//      print(v);
        IStmt ex9 = new CompStmt(
                new VarDeclStmt("v", new IntType()),
                new CompStmt(
                        new AssignStmt("v", new ValueExp(new IntValue(4))),
                        new CompStmt(
                                new WhileStmt(
                                        new RelExp(">", new VarExp("v"), new ValueExp(new IntValue(0))),
                                        new CompStmt(
                                                new PrintStmt(new VarExp("v")),
                                                new AssignStmt("v", new ArithExp('-', new VarExp("v"), new ValueExp(new IntValue(1))))
                                        )
                                ),
                                new PrintStmt(new VarExp("v"))
                        )
                )
        );
        MyIRepository repository9 = new MyFileRepository("log9.txt");
        PrgState prgState9 = new PrgState(ex9);
        repository9.addProgram(prgState9);
        MyController controller9 = new MyController(repository9);

        // int v;
        // Ref int a;
        // v = 10;
        // new(a, 22);
        // fork(wH(a, 30);
        // v = 32;
        // print(v);
        // print(rH(a)));
        // print(v);
        // print(rH(a));
        IStmt ex10 = new CompStmt(
                new VarDeclStmt("v", new IntType()),
                new CompStmt(
                        new VarDeclStmt("a", new RefType(new IntType())),
                        new CompStmt(
                                new AssignStmt("v", new ValueExp(new IntValue(10))),
                                new CompStmt(
                                        new NewStmt("a", new ValueExp(new IntValue(22))),
                                        new CompStmt(
                                                new ForkStmt(
                                                        new CompStmt(
                                                                new WriteHeapStmt("a", new ValueExp(new IntValue(30))),
                                                                new CompStmt(
                                                                        new AssignStmt("v", new ValueExp(new IntValue(32))),
                                                                        new CompStmt(
                                                                                new PrintStmt(new VarExp("v")),
                                                                                new PrintStmt(new ReadHeapExp(new VarExp("a")))
                                                                        )
                                                                )
                                                        )
                                                ),
                                                new CompStmt(
                                                        new PrintStmt(new VarExp("v")),
                                                        new PrintStmt(new ReadHeapExp(new VarExp("a")))
                                                )
                                        )
                                )
                        )
                )
        );
        MyIRepository repository10 = new MyFileRepository("log10test.txt");
        PrgState prgState10 = new PrgState(ex10);
        repository10.addProgram(prgState10);
        MyController controller10 = new MyController(repository10);

        TextMenu menu = new TextMenu();
        menu.addCommand(new ExitCommand("0", "exit"));
        menu.addCommand(new RunExampleCommand("1", ex1.toString(), controller1));
        menu.addCommand(new RunExampleCommand("2", ex2.toString(), controller2));
        menu.addCommand(new RunExampleCommand("3", ex3.toString(), controller3));
        menu.addCommand(new RunExampleCommand("4", ex4.toString(), controller4));
        menu.addCommand(new RunExampleCommand("5", ex5.toString(), controller5));
        menu.addCommand(new RunExampleCommand("6", ex6.toString(), controller6));
        menu.addCommand(new RunExampleCommand("7", ex7.toString(), controller7));
        menu.addCommand(new RunExampleCommand("8", ex8.toString(), controller8));
        menu.addCommand(new RunExampleCommand("9", ex9.toString(), controller9));
        menu.addCommand(new RunExampleCommand("10", ex10.toString(), controller10));
        menu.show();
    }
}
