package View;

import Model.Expressions.*;
import Model.Statements.*;
import Model.Types.*;
import Model.Values.*;

import java.util.ArrayList;
import java.util.List;

public class HardcodedPrograms {
    public static List<IStmt> getPrograms() {
        return new ArrayList<>(List.of(
                // Program 1: int v; print(v);
                new CompStmt(
                        new VarDeclStmt("v", new IntType()),
                        new PrintStmt(new VarExp("v"))
                ),

                // Program 2:
                // int a;
                // int b;
                // a = 2 + 3 * 5;
                // b = a + 1;
                // print(b);
                new CompStmt(
                        new VarDeclStmt("a", new IntType()),
                        new CompStmt(
                                new VarDeclStmt("b", new IntType()),
                                new CompStmt(
                                        new AssignStmt(
                                                "a",
                                                new ArithExp('+',
                                                        new ValueExp(new IntValue(2)),
                                                        new ArithExp('*', new ValueExp(new IntValue(3)), new ValueExp(new IntValue(5)))
                                                )
                                        ),
                                        new CompStmt(
                                                new AssignStmt(
                                                        "b",
                                                        new ArithExp('+', new VarExp("a"), new ValueExp(new IntValue(1)))
                                                ),
                                                new PrintStmt(new VarExp("b"))
                                        )
                                )
                        )
                ),

                // Program 3:
                // bool a;
                // int v;
                // a = true;
                // (If a Then v = 2 Else v = 3);
                // Print(v);
                new CompStmt(
                        new VarDeclStmt("a", new BoolType()),
                        new CompStmt(
                                new VarDeclStmt("v", new IntType()),
                                new CompStmt(
                                        new AssignStmt("a", new ValueExp(new BoolValue(true))),
                                        new CompStmt(
                                                new IfStmt(
                                                        new VarExp("a"),
                                                        new AssignStmt("v", new ValueExp(new IntValue(2))),
                                                        new AssignStmt("v", new ValueExp(new IntValue(3)))
                                                ),
                                                new PrintStmt(new VarExp("v"))
                                        )
                                )
                        )
                ),

                // Program 4:
                // string varf;
                // varf = "test.in";
                // openRFile(varf);
                // int varc;
                // readFile(varf, varc);
                // print(varc);
                // readFile(varf, varc);
                // print(varc);
                // closeRFile(varf);
                new CompStmt(
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
                ),

                // Program 5:
                // Ref int v;
                // new(v, 20);
                // Ref Ref int a;
                // new(a, v);
                // print(v);
                // print(a);
                new CompStmt(
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
                ),

                // Program 6:
                // Ref int v;
                // new(v, 20);
                // Ref Ref int a;
                // new(a, v);
                // print(rH(v));
                // print(rH(rH(a)) + 5);
                new CompStmt(
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
                ),

                // Program 7:
                // Ref int v;
                // new(v, 20);
                // print(rH(v));
                // wH(v, 30);
                // print(rH(v) + 5);
                new CompStmt(
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
                ),

                // Program 8:
                // Ref int v;
                // new(v, 20);
                // Ref Ref int a;
                // new(a, v);
                // new(v, 30);
                // print(rH(rH(a)));
                new CompStmt(
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
                ),

                // Program 9:
                // int v;
                // v = 4;
                // while(v > 0)
                //     print(v);
                //     v = v - 1;
                // print(v);
                new CompStmt(
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
                ),

                // Program 10:
                // int v;
                // Ref int a;
                // v = 10;
                // new(a, 22);
                // fork(wH(a, 30);
                // v = 32;
                // print(v);
                // print(rH(a));
                // print(v);
                // print(rH(a));
                new CompStmt(
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
                ),
                // Failing Program Example:
                // int x;
                // x = true;  // Type mismatch
                new CompStmt(
                        new VarDeclStmt("x", new IntType()),
                        new AssignStmt("x", new ValueExp(new BoolValue(true))) // This line will cause a type error
                )
        )
        );
    }
}
