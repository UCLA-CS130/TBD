# CS 130 Software Engineering, Winter 2017
## Assignment 9: Design Document
## Team TBD
* Alexander Waz
* Derek Nguyen
* Sung Hyun Yoon

 ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

# 1. Compression
### Feature Description
One of the features we implemented was HTTP compression. The encoding we chose was gzip. We serve static file and echo requests through compressing them.

### High-level Implementation Details
We made a Compressor class, where we have a Compress function that takes a string, and returns the compressed gzip format as a string.

    string Compressor::compress(const string& original) {
        create a stringstream for uncompressed and compressed
        instantiate boost filter stream
        push the gzip compressor to the filter stream
        push the uncompressed stringstream through the filter
        copy the compressed stringstream into the output
        return compressed string
    }

We chose gzip because it is one of the most widely used compression schemes, and is supported by Boost. In the static file handler and echo handler, we add the headers `Content-Encoding: gzip`, pass the original body through the Compress function, and then set the body equal to the compressed string.

### URL for demo
52.40.2.14/static2/test.txt (link to a static file, easier seen demonstrated by curl)

### Walkthrough
1. By using `$ curl -is 52.40.2.14/static2/test.txt`
2. This gives the compressed output of the static file
3. Then use `$ curl --compressed -is 52.40.2.14/static2/test.txt`
4. We now get the uncompressed plain text

 ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

# 2. Markdown
### Feature Description
We also implemented markdown rendering. When a file with a .md extension is requested, the file is transparently converted to and rendered as HTML.

### High-level Implementation Details
Most of the changes were made to our previous implementation of static file handler. Among the libraries that were suggested as aprt of the assignment specification, we decided to go with [cpp-markdown][1], since it was easy to integrate and relatively small. 

Below is a high-level abstraction of our implementation

    Status StaticHandler::HandleRequest(request, response) {
        // Read file content
        ...
        string content_type = GetMimeType(file_path);	    // set to text/html if .md
        if (IsMarkdown(file_path)) {				        // check if .md extension
            file_content = ProcessMarkdown(file_content);   // convert to html using cpp-markdown
        }
        // Write response
        ...
    }

### URL for demo
This design document is actually a .md file that is rendered as HTML!

 ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

# 3. HTTPS
### Feature Description
We attempted to implement HTTPS encryption between the client and the server to have a secured connection.

### High-level Implementation Details
We were not able to complete this feature on time, but did attempt to implement it using boost ssl library and example. We created a private key and a self-signed certificate using openssl. We were able to set up the socket for the encrypted communication successfully, but our handshaking procedure between the server and client did not work as intended. When the browser tries to connect to the server using https, the browser warns us of an unsecured connection and self-signed certificate. In order to see the response from the server, we had to add an exception to the browser, which is not safe.

Since we were not able to complete this feature, all of the code written for https implementation were not merged into master and can be viewed under the [https branch][2].

[1]: https://github.com/sevenjay/cpp-markdown
[2]: https://github.com/UCLA-CS130/TBD/tree/https
