#ifndef HUMAN_HPP
#define HUMAN_HPP

#include <cstddef>
#include <vector>

#include "math/mat4.hpp"
#include "math/vec3.hpp"

class Cube;
class MatrixStack;
class Shader;

enum class Motion
{
    Idle,
    Walk,
    Jump
};

/*
** Articulated character built as a tree of parts.
** Every child's joint is computed from its parent's dimensions, so changing
** a limb length automatically moves everything below it.
*/
class Human
{
public:
    Human();

    void update(float deltaTime);
    void setMotion(Motion motion);
    Motion motion() const;

    void draw(const Shader &shader, const Cube &cube) const;

    void setTorsoHeight(float height);
    void growTorso(float delta);
    void growArms(float delta);
    void growLegs(float delta);

private:
    enum PartIndex
    {
        Torso = 0,
        Head,
        LeftUpperArm,
        LeftForeArm,
        RightUpperArm,
        RightForeArm,
        LeftThigh,
        LeftShin,
        RightThigh,
        RightShin
    };

    struct Part
    {
        Vec3 pivot;   // joint position, relative to the parent's joint
        Vec3 size;    // dimensions of the 1x1x1 cube after scaling
        Vec3 offset;  // cube center, relative to this part's joint
        Vec3 color;
        int parent;
        float angleX;
        float angleY;
        float angleZ;

        Part() : parent(-1), angleX(0.0f), angleY(0.0f), angleZ(0.0f) {}
    };

    std::vector<Part> _parts;

    Vec3 _torsoSize;
    Vec3 _headSize;
    Vec3 _upperArmSize;
    Vec3 _foreArmSize;
    Vec3 _thighSize;
    Vec3 _shinSize;

    Motion _motion;
    float _time;
    float _jumpTime;
    float _bodyY;
    float _rootY;

    void rebuild();
    void resetPose();
    void drawPart(std::size_t index, MatrixStack &stack, const Shader &shader, const Cube &cube) const;
};

#endif
