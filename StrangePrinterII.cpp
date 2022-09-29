#include<iostream>
#include<vector>
#include<algorithm>
#include<tuple>
#include<string_view>
#include<set>
#include"my_assert.h"



//std::ostream& operator<<(std::ostream& out, const GridBounds& bounds) {
//    using namespace std::literals;
//    out << '{' << bounds.left << ", "sv << bounds.right << ", "sv << bounds.upper << ", "sv << bounds.lower << '}';
//    return out;
//}

class Solution {
    struct GridBounds {
        int left = -1;
        int right = -1;
        int upper = -1;
        int lower = -1;
        int color = -1;

        bool operator==(const GridBounds& rhs) const {
            return std::tie(left, right, upper, lower) == std::tie(rhs.left, rhs.right, rhs.upper, rhs.lower);
        }

        bool operator !=(const GridBounds& rhs) const {
            return !(*this == rhs);
        }
    };

public:
    bool isPrintable(std::vector<std::vector<int>>& targetGrid) {
        bool ret = true;
        for (int i = 0; i < targetGrid.size(); ++i) {
            if (ret == false) {
                return false;
            }
            auto it = std::find_if_not(targetGrid[i].begin(), targetGrid[i].end(), [](int color) {
                return color == -1;
                                       });
            if (it == targetGrid[i].end()) {
                if (i == targetGrid.size() - 1) {
                    break;
                }
                continue;
            }
            std::set<int> underlying_colors;
            ret = isSolid(targetGrid, GetBounds(targetGrid, *it), underlying_colors);
            i = 0;
        }
        return ret;
    }

private:
    GridBounds GetBounds(std::vector<std::vector<int>>& t, int color) {
        GridBounds bounds;
        bounds.color = color;
        auto Checker = [color](int col) {
            return col == color;
        };
        for (auto v_it = t.begin(); v_it != t.end(); ++v_it) {
            auto it = std::find_if(v_it->begin(), v_it->end(), Checker);
            if (it == v_it->end()) {
                continue;
            }
            if (bounds.upper == -1) {
                bounds.upper = v_it - t.begin();
            }
            bounds.lower = v_it - t.begin();
            bounds.left = bounds.left == -1 ? std::distance(v_it->begin(), it) : std::min(bounds.left, static_cast<int>(std::distance(v_it->begin(), it)));
            auto r_it = std::find_if(v_it->rbegin(), v_it->rend(), Checker);
            bounds.right = bounds.right == -1 ? std::distance(v_it->begin(), r_it.base() - 1) : std::max(bounds.right, static_cast<int>(std::distance(v_it->begin(), r_it.base() - 1)));
        }
        return bounds;
    }

    bool isSolid(std::vector<std::vector<int>>& t, GridBounds grid, std::set<int>& underlying_colors) {
        int this_color = grid.color;
        for (int y = grid.upper; y <= grid.lower; ++y) {
            for (int x = grid.left; x <= grid.right; ++x) {
                if (underlying_colors.count(t[y][x]) != 0) {
                    return false;
                }
                if (t[y][x] == -1) {
                    continue;
                }
                if (t[y][x] != this_color) {
                    underlying_colors.insert(this_color);
                    if (isSolid(t, GetBounds(t, t[y][x]), underlying_colors)) {
                        underlying_colors.erase(this_color);
                    }
                    else {
                        return false;
                    }
                }
            }
        }
        MarkDeleted(t, grid);
        return true;
    }

    void MarkDeleted(std::vector<std::vector<int>>& t, GridBounds grid) {
        for (int y = grid.upper; y <= grid.lower; ++y) {
            for (int x = grid.left; x <= grid.right; ++x) {
                t[y][x] = -1;
            }
        }
    }
};

//void GetBoundsTest() {
//    using namespace std::literals;
//    std::vector<std::vector<int>> t{{1, 1, 1, 1},
//                                    {2, 2, 1, 1},
//                                    {2, 4, 4, 3},
//                                    {3, 4, 5, 5},
//                                    {3, 3, 5, 5}};
//    Solution s;
//    GridBounds bounds_1 = s.GetBounds(t, 1);
//    GridBounds bounds_2 = s.GetBounds(t, 2);
//    GridBounds bounds_3 = s.GetBounds(t, 3);
//    GridBounds bounds_4 = s.GetBounds(t, 4);
//    GridBounds bounds_5 = s.GetBounds(t, 5);
//    GridBounds estimate_1 = {0, 3, 0, 1};
//    GridBounds estimate_2 = {0, 1, 1, 2};
//    GridBounds estimate_3 = {0, 3, 2, 4};
//    GridBounds estimate_4 = {1, 2, 2, 3};
//    GridBounds estimate_5 = {2, 3, 3, 4};
//    ASSERT_EQUAL(bounds_1, estimate_1);
//    ASSERT_EQUAL(bounds_2, estimate_2);
//    ASSERT_EQUAL(bounds_3, estimate_3);
//    ASSERT_EQUAL(bounds_4, estimate_4);
//    ASSERT_EQUAL(bounds_5, estimate_5);
//}
//
//void IsSolidTest() {
//    using namespace std::literals;
//    std::vector<std::vector<int>> t{{1, 1, 1, 1},
//                                    {2, 2, 1, 1},
//                                    {2, 4, 4, 3},
//                                    {3, 4, 5, 5},
//                                    {3, 3, 5, 5}};
//    Solution s;
//    std::set<int> colors{};
//    ASSERT(s.isSolid(t, s.GetBounds(t, 1), colors));
//    std::vector<std::vector<int>> estimate = {{-1, -1, -1, -1},
//                                              {-1, -1, -1, -1},
//                                              {-1, -1, -1, 3},
//                                              {3, -1, -1, -1},
//                                              {3, 3, -1, -1}};
//    ASSERT(t == estimate);
//}
//
void IsPrintableTest() {
    using namespace std::literals;
    {
        std::vector<std::vector<int>> t{{1, 1, 1, 1},
                                        {2, 2, 1, 1},
                                        {2, 4, 4, 3},
                                        {3, 4, 5, 5},
                                        {3, 3, 5, 5}};
        Solution s;
        ASSERT(s.isPrintable(t));
    }
    {
        std::vector<std::vector<int>> t{{1, 1, 1, 1},
                                        {2, 2, 1, 1},
                                        {2, 4, 4, 3},
                                        {3, 4, 5, 5},
                                        {3, 3, 5, 3}};
        Solution s;
        ASSERT(!s.isPrintable(t));
    }
    {
        std::vector<std::vector<int>> t{{6, 2, 2, 5},
                                        {2, 2, 2, 5},
                                        {2, 2, 2, 5},
                                        {4, 3, 3, 4}};
        Solution s;
        ASSERT(s.isPrintable(t));
    }
}

int main() {
    IsPrintableTest();
}
