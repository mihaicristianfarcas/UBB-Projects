package com.documentsAndMovies.model;

public class Author {
    private int id;
    private String name;
    private String documentList;
    private String movieList;

    public Author() {
        this.id = 0;
        this.name = "";
        this.documentList = "";
        this.movieList = "";
    }

    public Author(String name, String documentList, String movieList) {
        this.name = name;
        this.documentList = documentList;
        this.movieList = movieList;
    }

    public Author(String name) {
        this.name = name;
        this.documentList = "";
        this.movieList = "";
    }

    // Getters and Setters
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

    public String getDocumentList() {
        return documentList;
    }

    public void setDocumentList(String documentList) {
        this.documentList = documentList;
    }

    public String getMovieList() {
        return movieList;
    }

    public void setMovieList(String movieList) {
        this.movieList = movieList;
    }

    @Override
    public String toString() {
        return "Author{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", documentList='" + documentList + '\'' +
                ", movieList=" + movieList +
                '}';
    }
}
