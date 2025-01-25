#include "include/vector/TimestampColumnVector.h"

int main() {
    TimestampColumnVector tsVector(10, 0, true);  // 创建一个带时间戳的列向量

    // 添加一些时间戳
    long ts1 = 1672531200000;  // 示例时间戳
    long ts2 = 1672617600000;
    tsVector.add(&ts1);
    tsVector.add(&ts2);

    // 打印时间戳
    tsVector.print(2);  // 应输出 ts1 和 ts2 的值

    return 0;
}

