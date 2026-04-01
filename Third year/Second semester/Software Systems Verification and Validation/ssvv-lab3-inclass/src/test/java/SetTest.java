import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class SetTest {

    @Test
    void isInTheSet_empty_returnsFalse() {
        Set s = new Set(5);
        assertFalse(s.IsInTheSet(5));
    }

    @Test
    void isInTheSet_single_has3_returnsTrue() {
        Set s = new Set(5);
        s.AddAValue(3);
        assertTrue(s.IsInTheSet(3));
    }

    @Test
    void isInTheSet_single_has3_check5_returnsFalse() {
        Set s = new Set(5);
        s.AddAValue(3);
        assertFalse(s.IsInTheSet(5));
    }

    @Test
    void isInTheSet_threeElements_contains3_returnsTrue() {
        Set s = new Set(5);
        s.AddAValue(1);
        s.AddAValue(2);
        s.AddAValue(3);
        assertTrue(s.IsInTheSet(3));
    }

    @Test
    void isInTheSet_threeElements_check7_returnsFalse() {
        Set s = new Set(5);
        s.AddAValue(1);
        s.AddAValue(2);
        s.AddAValue(3);
        assertFalse(s.IsInTheSet(7));
    }

    @Test
    void isInTheSet_threeElements_contains1_returnsTrue() {
        Set s = new Set(5);
        s.AddAValue(1);
        s.AddAValue(2);
        s.AddAValue(3);
        assertTrue(s.IsInTheSet(1));
    }

    @Test
    void addAValue_empty_addsAndReturnsTrue() {
        Set s = new Set(5);
        assertTrue(s.AddAValue(5));
        assertEquals(1, s.nVS);
        assertEquals(5, s.vS[0]);
    }

    @Test
    void addAValue_duplicate_returnsFalse_and_nVSUnchanged() {
        Set s = new Set(5);
        s.AddAValue(3);
        assertFalse(s.AddAValue(3));
        assertEquals(1, s.nVS);
        assertEquals(3, s.vS[0]);
    }

    @Test
    void addAValue_multiple_adds_increments_nVS_and_storesValues() {
        Set s = new Set(5);
        assertTrue(s.AddAValue(1));
        assertTrue(s.AddAValue(2));
        assertTrue(s.AddAValue(3));
        assertEquals(3, s.nVS);
        assertEquals(2, s.vS[1]);
    }

    @Test
    void addAValue_beyondCapacity_throws() {
        Set s = new Set(2);
        s.AddAValue(1);
        s.AddAValue(2);
        assertThrows(ArrayIndexOutOfBoundsException.class, () -> s.AddAValue(3));
    }
}
