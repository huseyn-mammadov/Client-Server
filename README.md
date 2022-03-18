# Client-Server
High-level Multi-Thread Server 


C and C++ Programming languages




wordHistogramServer.c - This is a client-server application. At a high level, the client program wordHistogramClient connect()s to the server wordHistogramServer. When the server accept()s a client via a socket the server makes a child thread to handle the new client



histogrammer.cpp - opens the file named file.txt, fast-forwards to the index word, and counts the next count words it sees. It uses 2 threads:

    The parent thread:
    1. opens the file,
    2. fast-forwards to the word indexed by wordIndex,
    3. makes the child thread
    Then it runs a loop that makes it read one word per second. The global variable wordPtr is set pointing to this word. Then, it signals the child thread           to count this word.
    
The child thread:
  runs histogramMaker(). It updates a locally-stored binary tree of Node* instances to node that the word was read. It sets wordPtr to NULL. Then, it signals the parent thread that it may read the next word.

When the histogrammer process receives SIGINT the reading thread stops reading. Also, the counting thread stops counting. The counting thread outputs counts so far by walking its sorted tree of word counts and printf()ing to stdout, (which is really the child-to-parent pipe), and then quits.





