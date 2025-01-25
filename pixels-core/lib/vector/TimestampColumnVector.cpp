//
// Created by liyu on 12/23/23.
//

#include "vector/TimestampColumnVector.h"

TimestampColumnVector::TimestampColumnVector(int precision, bool encoding): ColumnVector(VectorizedRowBatch::DEFAULT_SIZE, encoding) {
    TimestampColumnVector(VectorizedRowBatch::DEFAULT_SIZE, precision, encoding);
}

TimestampColumnVector::TimestampColumnVector(uint64_t len, int precision, bool encoding): ColumnVector(len, encoding) {
    this->precision = precision;
    if(encoding) {
        posix_memalign(reinterpret_cast<void **>(&this->times), 64,
                       len * sizeof(long));
    } else {
        this->times = nullptr;
    }
}


void TimestampColumnVector::close() {
    if(!closed) {
        ColumnVector::close();
        if(encoding && this->times != nullptr) {
            free(this->times);
        }
        this->times = nullptr;
    }
}

void TimestampColumnVector::print(int rowCount) {
    throw InvalidArgumentException("not support print longcolumnvector.");
//    for(int i = 0; i < rowCount; i++) {
//        std::cout<<longVector[i]<<std::endl;
//		std::cout<<intVector[i]<<std::endl;
//    }
}

TimestampColumnVector::~TimestampColumnVector() {
    if(!closed) {
        TimestampColumnVector::close();
    }
}

void * TimestampColumnVector::current() {
    if(this->times == nullptr) {
        return nullptr;
    } else {
        return this->times + readIndex;
    }
}

/**
     * Set a row from a value, which is the days from 1970-1-1 UTC.
     * We assume the entry has already been isRepeated adjusted.
     *
     * @param elementNum
     * @param days
 */
void TimestampColumnVector::set(int elementNum, long ts) {
    if(elementNum >= writeIndex) {
        writeIndex = elementNum + 1;
    }
    times[elementNum] = ts;
    // TODO: isNull
    int byteIndex = elementNum / 8;
    int bitIndex = elementNum % 8;
    auto* isNullByte = reinterpret_cast<uint8_t*>(isNull);
    isNullByte[byteIndex] &= ~(1 << bitIndex);  // 设置为非空
}



void TimestampColumnVector::add(void* value) {
    long timestampValue = *(static_cast<long*>(value));  // 将 void* 转换为 long 类型
    ensureSize(writeIndex + 1);  // 确保数组大小足够
    times[writeIndex++] = timestampValue;  // 添加值
}

void TimestampColumnVector::ensureSize(uint64_t size) {
    if (size > length) {
        // 扩展数组大小，默认两倍扩展策略
        uint64_t newSize = std::max(size, length * 2);
        long* newTimes;
        posix_memalign(reinterpret_cast<void**>(&newTimes), 64, newSize * sizeof(long));
        if (times != nullptr) {
            std::copy(times, times + writeIndex, newTimes);  // 拷贝旧数据
            free(times);  // 释放旧内存
        }
        times = newTimes;
        length = newSize;
    }
}
