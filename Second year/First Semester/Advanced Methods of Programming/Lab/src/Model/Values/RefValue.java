package Model.Values;

import Model.Types.RefType;
import Model.Types.Type;

import java.util.Objects;

public class RefValue implements Value {
    private int address;
    private Type locationType;

    public RefValue(int address, Type locationType) {
        this.address = address;
        this.locationType = locationType;
    }

    public int getAddress() {
        return address;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof RefValue) {
            return Objects.equals(((RefValue) obj).getAddress(), address);
        }
        return false;
    }

    @Override
    public Type getType() {
        return new RefType(locationType);
    }

    @Override
    public Value deepCopy() {
        return null;
    }

    @Override
    public String toString() {
        return "(" + address + ", " + locationType + ")";
    }
}
