package ssvv.lab2.inclass;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;
import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class AppTest {
    private final ByteArrayOutputStream outContent = new ByteArrayOutputStream();
    private final PrintStream originalOut = System.out;

    @AfterEach
    public void tearDown() {
        System.setOut(originalOut);
        new File("in.txt").delete();
    }

    private void createInFile(String content) throws IOException {
        Files.write(Paths.get("in.txt"), content.getBytes());
    }

    private String runMain() {
        System.setOut(new PrintStream(outContent));
        App.main(new String[]{});
        return outContent.toString().trim();
    }

    @Test
    public void testEmptyArray() throws IOException {
        createInFile("0");
        assertEquals("0", runMain());
    }

    @Test
    public void testAllOdd() throws IOException {
        createInFile("3\n1\n3\n5");
        assertEquals("0", runMain());
    }

    @Test
    public void testMixedEvenOdd() throws IOException {
        createInFile("5\n1\n2\n3\n4\n6");
        assertEquals("3", runMain());
    }

    @Test
    public void testMaxIntValue() throws IOException {
        createInFile("1\n2147483647"); // Odd
        assertEquals("0", runMain());
    }

    @Test
    public void testBoundaryEvenValue() throws IOException {
        createInFile("1\n2147483646"); // Even
        assertEquals("1", runMain());
    }

    @Test
    public void testZero() throws IOException {
        createInFile("1\n0"); // Even
        assertEquals("1", runMain());
    }
}
