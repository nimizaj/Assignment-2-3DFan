#define LOG_TAG "Triangle"
#include "Triangle.h"
#include "ShaderHelper.h"

// Clip-space triangle positions (same as the original Cookbook values)
static const GLfloat kPositions[] = {
     0.0f,  0.5f,   // top
    -0.5f, -0.5f,   // bottom-left
     0.5f, -0.5f    // bottom-right
};

// Attribute locations match layout qualifiers in the GLSL shaders
static constexpr GLuint ATTRIB_POSITION = 0;
static constexpr GLuint ATTRIB_COLOR    = 1;

// ---------------------------------------------------------------------------
// Constructors / Destructor
// ---------------------------------------------------------------------------

#ifdef PLATFORM_ANDROID
Triangle::Triangle(AAssetManager* assetMgr)
    : mgr(assetMgr)
{
#else
Triangle::Triangle()
{
#endif
    modelType = TriangleType;
    setAllColors(0.0f, 0.0f, 1.0f);   // Start blue
}

Triangle::~Triangle()
{
    if (vao)    { glDeleteVertexArrays(1, &vao);    vao    = 0; }
    if (vboPos) { glDeleteBuffers(1, &vboPos);      vboPos = 0; }
    if (vboCol) { glDeleteBuffers(1, &vboCol);      vboCol = 0; }
    if (program){ glDeleteProgram(program);          program = 0; }
}

// ---------------------------------------------------------------------------
// InitModel – compile shaders, create VAO/VBO
// ---------------------------------------------------------------------------
void Triangle::InitModel()
{
    LOGI("Triangle::InitModel");

#ifdef PLATFORM_ANDROID
    program = ShaderHelper::buildProgramFromAssets(
        mgr,
        "shader/TriangleVertex.glsl",
        "shader/TriangleFragment.glsl");
#else
    // Desktop (GLFW) and WebGL (Emscripten): load from filesystem / virtual FS.
    // The build system copies / preloads the shader files so they are reachable
    // at assets/shader/<name> relative to the working directory.
    program = ShaderHelper::buildProgramFromFile(
        "TriangleVertex.glsl",
        "TriangleFragment.glsl");
#endif

    if (!program) {
        LOGE("Triangle: failed to build shader program");
        return;
    }

    // --- VAO ---
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // --- Position VBO (static) ---
    glGenBuffers(1, &vboPos);
    glBindBuffer(GL_ARRAY_BUFFER, vboPos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kPositions), kPositions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(ATTRIB_POSITION);
    glVertexAttribPointer(ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    // --- Colour VBO (dynamic) ---
    glGenBuffers(1, &vboCol);
    glBindBuffer(GL_ARRAY_BUFFER, vboCol);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(ATTRIB_COLOR);
    glVertexAttribPointer(ATTRIB_COLOR, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    LOGI("Triangle::InitModel done");
}

// ---------------------------------------------------------------------------
// Render
// ---------------------------------------------------------------------------
void Triangle::Render()
{
    if (!program || !vao) return;
    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

// ---------------------------------------------------------------------------
// Touch / mouse events
// ---------------------------------------------------------------------------
void Triangle::TouchEventDown(float, float)    { setAllColors(1,0,0); uploadColors(); }
void Triangle::TouchEventMove(float, float)    { setAllColors(0,1,0); uploadColors(); }
void Triangle::TouchEventRelease(float, float) { setAllColors(0,0,1); uploadColors(); }

// ---------------------------------------------------------------------------
void Triangle::setAllColors(float r, float g, float b)
{
    for (int v = 0; v < 3; ++v) {
        colors[v*3+0] = r;
        colors[v*3+1] = g;
        colors[v*3+2] = b;
    }
}

void Triangle::uploadColors()
{
    if (!vboCol) return;
    glBindBuffer(GL_ARRAY_BUFFER, vboCol);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors), colors);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
