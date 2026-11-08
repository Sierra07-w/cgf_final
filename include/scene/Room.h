#pragma once

class Room {
public:
    Room();
    ~Room();

    Room(const Room&) = delete;
    Room& operator=(const Room&) = delete;

    void draw() const;

private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    int vertexCount = 0;
};
