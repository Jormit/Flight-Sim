#include "Mesh.hpp"

using namespace std;

// Initialise with texture list.
Mesh::Mesh(const std::string& filename, const std::string& base, std::vector<std::string> df_textures,
        const char *vectFile, const char *fragFile) :
meshShader(vectFile, fragFile), modelMatrix(1.0f) {
    vector<std::string> temp(df_textures);
    this->df_textures = temp;
    setupTextures();
    loadFile(filename, base);
    numShapes = shapes.size();
    setupBuffers();
}

// Initialise without textures.
Mesh::Mesh(const std::string& filename, const std::string& base, const char *vectFile, const char *fragFile) :
meshShader(vectFile, fragFile), modelMatrix(1.0f) {
    loadFile(filename, base);
    numShapes = shapes.size();
    setupBuffers();
}

// Loads file using tiny obj loader and prints out some info.
void Mesh::loadFile(const std::string& filename, const std::string& base) {
    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), base.c_str());

    if (!warn.empty()) {
        std::cout << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        exit(1);
    }

    cout << filename << " loaded." << endl;
    printf("# of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
    printf("# of normals   = %d\n", (int)(attrib.normals.size()) / 3);
    printf("# of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
    printf("# of materials = %d\n", (int)materials.size());
    printf("# of shapes    = %d\n", (int)shapes.size());
}

// Organise tiny obj data into opengl buffers.
// Not super efficient memory wise.
void Mesh::setupBuffers() {
    // Loop over shapes

    for (size_t s = 0; s < numShapes; s++) {
        // Loop over faces(polygon)
        std::vector<float> vertexBuffer;
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
                tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
                tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
                tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
                tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];

                float arr[] = {vx, vy, vz, nx, ny, nz, tx, ty};
                vertexBuffer.insert(vertexBuffer.end(), begin(arr), end(arr));
            }
            index_offset += fv;
        }

        buffer newBuffer{};

        glGenVertexArrays(1, &(newBuffer.VAO));
        glGenBuffers(1, &(newBuffer.VBO));

        glBindVertexArray(newBuffer.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, newBuffer.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexBuffer.size(), vertexBuffer.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        newBuffer.size = vertexBuffer.size() / 8;
        glBuffers.push_back(newBuffer);
    }

    meshShader.setInt("objTexture", 0);
}

void Mesh::setupTextures() {
    for (int i = 0; i < df_textures.size(); i++) {
        textures.emplace_back(Texture(df_textures[i].c_str()));
    }
}

void Mesh::draw(glm::mat4 viewMat, glm::mat4 projMat) {

    // Draw each object seperately.
    for (int i = 0; i < numShapes; i++) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[i].getId());

        meshShader.use();
        meshShader.setMat4("view", viewMat);
        meshShader.setMat4("projection", projMat);
        meshShader.setMat4("model", modelMatrix);

        glBindVertexArray(glBuffers[i].VAO);
        glDrawArrays(GL_TRIANGLES, 0, glBuffers[i].size);
    }
}

void Mesh::setModelTransformation(glm::mat4 matrix) {
    this->modelMatrix = matrix;
}