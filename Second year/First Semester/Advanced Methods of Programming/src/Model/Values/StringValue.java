package Model.Values;

import Model.Types.StringType;
import Model.Types.Type;

import java.util.Objects;

public class StringValue implements Value {
    private String val;

    public StringValue(String v) {
        val = v;
    }

    public String getVal() {
        return val;
    }

    @Override
    public Type getType() {
        return new StringType();
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof StringValue) {
            return Objects.equals(((StringValue) obj).getVal(), val);
        }
        return false;
    }

    @Override
    public Value deepCopy() {
        return new StringValue(val);
    }

    @Override
    public String toString() {
        return val;
    }
}
