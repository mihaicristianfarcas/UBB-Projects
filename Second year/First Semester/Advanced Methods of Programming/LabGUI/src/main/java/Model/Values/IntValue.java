package Model.Values;

import Model.Types.IntType;
import Model.Types.Type;

public class IntValue implements Value {
    private final int val;

    public IntValue(int v) {
        val = v;
    }

    public int getVal() {
        return val;
    }

    @Override
    public Type getType() {
        return new IntType();
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof IntValue) {
            return ((IntValue) obj).getVal() == val;
        }
        return false;
    }

    @Override
    public Value deepCopy() {
        return new IntValue(val);
    }

    @Override
    public String toString() {
        return Integer.toString(val);
    }
}
