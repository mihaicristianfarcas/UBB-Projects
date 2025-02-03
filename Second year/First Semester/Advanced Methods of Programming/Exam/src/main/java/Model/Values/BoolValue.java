package Model.Values;

import Model.Types.BoolType;
import Model.Types.Type;

public class BoolValue implements Value {
    private final boolean val;

    public BoolValue(boolean v) {
        val = v;
    }

    public boolean getVal() {
        return val;
    }

    @Override
    public Type getType() {
        return new BoolType();
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof BoolValue) {
            return ((BoolValue) obj).getVal() == val;
        }
        return false;
    }

    @Override
    public Value deepCopy() {
        return new BoolValue(val);
    }

    @Override
    public String toString() {
        return Boolean.toString(val);
    }
}
