package com.exam_jsp.servlet;

import javax.servlet.*;
import java.io.IOException;

/**
 * A simple filter that sets the character encoding for requests and responses.
 * This is a more portable alternative to container-specific filters.
 */
public class CharacterEncodingFilter implements Filter {
    private String encoding;

    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        encoding = filterConfig.getInitParameter("encoding");
        if (encoding == null) {
            encoding = "UTF-8"; // Default encoding
        }
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain)
            throws IOException, ServletException {
        
        // Set encoding for both request and response if not already set
        if (request.getCharacterEncoding() == null) {
            request.setCharacterEncoding(encoding);
        }
        
        response.setCharacterEncoding(encoding);
        
        // Continue processing the request
        chain.doFilter(request, response);
    }

    @Override
    public void destroy() {
        // No resources to release
    }
}
