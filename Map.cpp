#include "Map.hpp"
using namespace std;

// Data for perlin noise generator.
static int SEED = 20;
static int mHash[] = {208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
                      185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
                      9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
                      70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
                      203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
                      164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
                      228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
                      232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
                      193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
                      101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
                      135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
                      114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219};

Map::Map (int size, float freq, int depth, int heightScale, int seed, int tileSize) :
shader("../shaders/map/vect.glsl", "../shaders/map/frag.glsl"),
texture1 ("../assets/landscape/frozenmarshlichen01.bmp"),
texture2 ("../assets/landscape/pineforest03.bmp"),
texture3 ("../assets/landscape/cavebaseground01.bmp"),
texture4 ("../assets/landscape/rocks01.bmp"),
tree1(R"(..\assets\tree\Fir.obj)", R"(C:\Users\jordan\ClionProjects\FlightSim\assets\tree)",
      vector<string>{"../assets/tree/branches.tga", "../assets/tree/trunk.tga"},
      "../shaders/tree/vect.glsl", "../shaders/tree/frag.glsl")
{
    // Enable the 4 textures for splatting.
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    shader.setInt("texture3", 2);
    shader.setInt("texture4", 3);

    tree1.setModelTransformation(glm::translate(glm::vec3(20.0f, 10.0f, -20.0f))  * glm::scale(glm::vec3(0.01f, 0.01f, 0.01f)));

    std::vector<Vertex> vertices;
    std::vector<int> index;
    this->size = size;

    // Generate vertices from perlin noise.
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            vertices.push_back(getVertex(glm::vec2((float)x, (float)y), freq, depth, heightScale, tileSize, seed));
        }
    }

    // Setup indices to match with vertices.
    for (int y = 0; y < size - 1; y++) {
        for (int x = 0; x < size - 1; x++) {
            index.push_back(x + size * y);
            index.push_back(x + 1 + size * y);
            index.push_back(size * (y + 1) + x);

            index.push_back(x + 1 + size * y);
            index.push_back(size * (y + 1) + x);
            index.push_back(size * (y + 1) + 1 + x);
        }
    }

    // Setup the buffers.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(int), index.data(), GL_STATIC_DRAW);

    // Vertex, normal, texture attributes.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Splat attribute.
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Generates a single vertex from perlin noise.
Vertex Map::getVertex(const glm::vec2 position, float freq, int depth, int heightScale, int tileSize, int seed) {
    Vertex v;
    float raw_h = perlin2d(position.x, position.y, freq, depth, seed);
    float h = heightScale * raw_h;
    float hx = 100.f * (heightScale * perlin2d(position.x + 0.01f, position.y, freq, depth, seed) - h);
    float hy = 100.f * (heightScale * perlin2d(position.x, position.y + 0.01f, freq, depth, seed) - h);

    v.position = glm::vec3(position.x, h, -position.y);
    v.normal = glm::normalize(glm::vec3(-hx, 1.0, -hy));

    // Texture tiling.
    float textureX = (float) tileSize * position.x/(float)size;
    float textureY = (float) tileSize * position.y/(float)size;
    v.texture = glm::vec2(textureX, textureY);

    v.splat = glm::vec4(pow(perlin2d(position.x, position.y, freq, depth, seed), 3),
                        pow(perlin2d(position.x, position.y, freq, depth, seed + 3939),3),
                        pow(perlin2d(position.x, position.y, freq, depth, seed + 232),3),
                        pow(perlin2d(position.x, position.y, freq, depth, seed + 32894), 3));
    return v;
}

// Draw map and any sub elements.
void Map::draw(glm::mat4 viewMat, glm::mat4 projMat, glm::vec4 lightPos){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1.getId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2.getId());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture3.getId());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture4.getId());

    shader.use();
    shader.setMat4("view", viewMat);
    shader.setMat4("projection", projMat);
    shader.setVec4("inLightPos", lightPos);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (size - 1) * (size - 1) * 2 * 3, GL_UNSIGNED_INT, NULL);

    tree1.draw(viewMat, projMat);

}

// Noise generation ALGORITHMS (Taken from github).
// https://gist.github.com/nowl/828013.
// ===================================================
int noise2(int x, int y)
{
    int tmp = mHash[(y + SEED) % 256];
    return mHash[(tmp + x) % 256];
}

float lin_inter(float x, float y, float s)
{
    return x + s * (y-x);
}

float smooth_inter(float x, float y, float s)
{
    return lin_inter(x, y, s * s * (3-2*s));
}

float noise2d(float x, float y)
{
    int x_int = x;
    int y_int = y;
    float x_frac = x - x_int;
    float y_frac = y - y_int;
    int s = noise2(x_int, y_int);
    int t = noise2(x_int+1, y_int);
    int u = noise2(x_int, y_int+1);
    int v = noise2(x_int+1, y_int+1);
    float low = smooth_inter(s, t, x_frac);
    float high = smooth_inter(u, v, x_frac);
    return smooth_inter(low, high, y_frac);
}

float perlin2d(float x, float y, float freq, int depth, int seed)
{
    SEED = seed;
    float xa = x*freq;
    float ya = y*freq;
    float amp = 1.0;
    float fin = 0;
    float div = 0.0;

    int i;
    for(i=0; i<depth; i++)
    {
        div += 256 * amp;
        fin += noise2d(xa, ya) * amp;
        amp /= 2;
        xa *= 2;
        ya *= 2;
    }

    return fin/div;
}

