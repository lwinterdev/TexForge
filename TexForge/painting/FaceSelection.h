
#pragma once

#include <unordered_set>

class FaceSelection
{
public:

    void Select(int meshIndex, int triangleIndex);

    void Deselect(int meshIndex, int triangleIndex);

    void Toggle(int meshIndex, int triangleIndex);

    void Clear();

    bool IsSelected(
        int meshIndex,
        int triangleIndex
    ) const;

private:

    std::unordered_set<long long> selectedFaces;
};