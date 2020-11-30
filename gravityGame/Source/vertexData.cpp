#include "vertexData.h"

VertexData::VertexData(const char* modelPath, int width, int height) {
    using json = nlohmann::json;
    this->width = width;
    this->height = height;
    std::unique_ptr<ConvertToFloat> conversion{ new ConvertToFloat(width, height) };
    std::unique_ptr<LoadFile> file{ new LoadFile() };
    std::string jsonString;
    jsonString = file->load(modelPath).str();
    json jf = json::parse(jsonString);
    float* vertices = new float[jf["vertices"].size() * 8];
    int* indices = new int[jf["indices"].size()];
    for (int i = 0; i < jf["vertices"].size(); i++)
        vertices[i] = jf["vertices"][i];
    for (int i = 0; i < jf["indices"].size(); i++)
        indices[i] = jf["indices"][i];
    



    conversion->format(vertices, jf["vertices"].size());
    //binds id
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, jf["vertices"].size()*8 *sizeof(float), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, jf["indices"].size()*4, indices, GL_STATIC_DRAW);
    //texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

VertexData::VertexData(const VertexData& data) {
    VAO = data.VAO;
    VBO = data.VBO;
    EBO = data.EBO;
    width = data.width;
    height = data.height;
    trans = data.trans;
}

void VertexData::render() {
    shader->use();
    unsigned int transformLoc = glGetUniformLocation(shader->ID, "location");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void VertexData::move(int x, int y) {
    this->x = x;
    this->y = y;
    float coor[2] = { float(x),float(y) };
    std::unique_ptr<ConvertToFloat> conversion{ new ConvertToFloat(width,height) };
    conversion->convertToGlobal(coor);
    glm::mat4 temp = glm::mat4(1.0f);
    temp = glm::translate(temp, glm::vec3(coor[0],coor[1], 0.0f));
    trans = temp;
}

int VertexData::getX() {
    return x;
}

int VertexData::getY() {
    return y;
}

void VertexData::rotate(int deg) {

}

void VertexData::destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}