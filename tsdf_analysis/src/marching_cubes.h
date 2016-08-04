#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H

#include <vector>

#ifndef WIN64
    #define EIGEN_DONT_ALIGN_STATICALLY
#endif
#include <Eigen/Dense>

#include <map>

typedef Eigen::Vector3d Vec3;
typedef Eigen::Vector3f Vec3f;
typedef Eigen::Vector3i Vec3i;
typedef Eigen::Matrix<unsigned char, 3, 1> Vec3b;

class MarchingCubes
{
public:
    MarchingCubes(const Vec3i &dimensions, const Vec3f &size);

    ~MarchingCubes();

    bool computeIsoSurface(const float* tsdf, const unsigned char* colors, float isoValue = 0.0f);

    size_t numVertices() const;
    size_t numFaces() const;
    
    bool savePly(const std::string &filename, bool saveColor = true) const;

protected:
    inline int computeLutIndex(int i, int j, int k, float isoValue);

    Vec3f interpolate(float tsdf0, float tsdf1, const Vec3f &val0, const Vec3f &val1, float isoValue);

    Vec3f voxelToWorld(const Vec3f &voxel) const;

    std::vector<Vec3b> m_colors;
    std::vector<Vec3i> m_faces;
    Vec3i m_dim;
    Vec3f m_size;
    Vec3f m_voxelSize;
public:
    std::vector<Vec3f> m_vertices;
};

#endif
