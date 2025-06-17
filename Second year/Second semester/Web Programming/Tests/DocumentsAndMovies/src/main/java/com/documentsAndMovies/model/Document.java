package com.documentsAndMovies.model;

public class Document {
    private int id;
    private String name;
    private String contents;

    public Document(String name, String contents) {
        this.name = name;
        this.contents = contents;
    }

    public Document() {
        this.name = "";
        this.contents = "";
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getContents() {
        return contents;
    }

    public void setContents(String contents) {
        this.contents = contents;
    }

    @Override
    public String toString() {
        return "Document{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", contents='" + contents + '\'' +
                '}';
    }
}
