package Model.Values;

import Model.Types.Type;

public interface Value {
    Type getType();
    boolean equals(Object obj);
    Value deepCopy();
    String toString();
}
