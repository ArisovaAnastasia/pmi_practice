#include "..\gtest\gtest.h"
#include <string>
#include <vector>

#include "vector.h"

void Check(const Vector& actual, const std::vector<int>& expected) {
    EXPECT_EQ(actual.Size(), expected.size());
    for (size_t i = 0; i < actual.Size(); ++i) {
        EXPECT_EQ(actual[i], expected[i]);
    }
}

class MemoryLeakDetector {
public:
    MemoryLeakDetector() {
        _CrtMemCheckpoint(&memState_);
    }

    ~MemoryLeakDetector() {
        _CrtMemState stateNow, stateDiff;
        _CrtMemCheckpoint(&stateNow);
        int diffResult = _CrtMemDifference(&stateDiff, &memState_, &stateNow);
        if (diffResult)
            reportFailure(stateDiff.lSizes[1]);
    }
private:
    void reportFailure(unsigned int unfreedBytes) {
        FAIL() << "Memory leak of " << unfreedBytes << " byte(s) detected.";
    }
    _CrtMemState memState_;
};

TEST(vector, test1) {
    MemoryLeakDetector leakDetector;
    {
        Vector a;
        EXPECT_EQ(a.Size(), 0u);
    }
    {
        Vector a{1, 2, 3, 4};
        Check(a, std::vector<int>{1, 2, 3, 4});
    }
    {
        Vector a(5);
        Check(a, std::vector<int>(5));
    }
}

TEST(vector, test2) {
    MemoryLeakDetector leakDetector;
    Vector a{1, 3, 5};
    EXPECT_EQ(a.Capacity(), 3u);
    Check(a, std::vector<int>{1, 3, 5});

    a.PopBack();
    Check(a, std::vector<int>{1, 3});
    a.Clear();
    EXPECT_EQ(a.Capacity(), 3u);
    Check(a, std::vector<int>());

    a.PushBack(6);
    Check(a, std::vector<int>{6});

    Vector b{3, 4};
    a.Swap(b);
    Check(a, std::vector<int>{3, 4});
    Check(b, std::vector<int>{6});
    EXPECT_EQ(a.Capacity(), 2u);
    EXPECT_EQ(b.Capacity(), 3u);
}

TEST(vector, test3) {
    MemoryLeakDetector leakDetector;
    Vector a{3, 7, 8};
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    Check(a, std::vector<int>{1, 2, 3});
}


TEST(vector, test4) {
    MemoryLeakDetector leakDetector;
    const int steps = 16;
    Vector data;
    for (int step = 0; step <= steps; ++step) {
        std::vector<int> ok_data;
        for (int i = 0; i <= (1 << step); ++i) {
            data.PushBack(i);
            ok_data.push_back(i);
        }
        Check(data, ok_data);
        EXPECT_EQ(data.Capacity(), (1u << (step + 1)));
        data.Clear();
    }
}

TEST(vector, test5) {
    MemoryLeakDetector leakDetector;
    Vector a;
    a.Reserve(5);
    EXPECT_EQ(a.Size(), 0u);
    EXPECT_EQ(a.Capacity(), 5u);

    for (int i = 0; i < 5; ++i) {
        a.PushBack(i);
    }

    EXPECT_EQ(a.Capacity(), 5u);
    a.PushBack(4);
    EXPECT_EQ(a.Capacity(), 10u);
    a.Reserve(3);
    EXPECT_EQ(a.Capacity(), 10u);
    a.Clear();
    EXPECT_EQ(a.Size(), 0u);
    EXPECT_EQ(a.Capacity(), 10u);
}


TEST(vector, test6) {
    MemoryLeakDetector leakDetector;
    Vector a;
    Vector b(a);
    b.PushBack(1);
    Check(a, std::vector<int>());
    Check(b, std::vector<int>{1});

    b = b;
    Check(b, std::vector<int>{1});
    a = b;
    Check(a, std::vector<int>{1});
    Check(b, std::vector<int>{1});

    Vector d{3, 4, 5};
    Vector e(d);
    {
        Vector copy = e;
    }
    Check(e, std::vector<int>{3, 4, 5});
}
