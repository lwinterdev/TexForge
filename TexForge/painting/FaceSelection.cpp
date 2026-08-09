#include "FaceSelection.h"

// Convert a mesh index + triangle index into one unique value.
//
// Example:
// mesh  = 2
// face  = 15
//
// becomes one key that can be stored in unordered_set.
static long long MakeFaceKey(
    int meshIndex,
    int triangleIndex)
{
    return
        (static_cast<long long>(meshIndex) << 32) |
        static_cast<unsigned int>(triangleIndex);
}


void FaceSelection::Select(
    int meshIndex,
    int triangleIndex)
{
    long long key =
        MakeFaceKey(
            meshIndex,
            triangleIndex
        );

    selectedFaces.insert(key);
}


void FaceSelection::Deselect(
    int meshIndex,
    int triangleIndex)
{
    long long key =
        MakeFaceKey(
            meshIndex,
            triangleIndex
        );

    selectedFaces.erase(key);
}


void FaceSelection::Toggle(
    int meshIndex,
    int triangleIndex)
{
    long long key =
        MakeFaceKey(
            meshIndex,
            triangleIndex
        );

    auto result =
        selectedFaces.find(key);

    if (result != selectedFaces.end())
    {
        // Already selected → deselect it
        selectedFaces.erase(key);
    }
    else
    {
        // Not selected → select it
        selectedFaces.insert(key);
    }
}


void FaceSelection::Clear()
{
    selectedFaces.clear();
}


bool FaceSelection::IsSelected(
    int meshIndex,
    int triangleIndex) const
{
    long long key =
        MakeFaceKey(
            meshIndex,
            triangleIndex
        );

    return
        selectedFaces.find(key)
        != selectedFaces.end();
}