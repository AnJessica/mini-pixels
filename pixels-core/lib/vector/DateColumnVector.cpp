//
// Created by yuly on 06.04.23.
//

#include "vector/DateColumnVector.h"

DateColumnVector::DateColumnVector(uint64_t len, bool encoding): ColumnVector(len, encoding) {
	if(encoding) {
        posix_memalign(reinterpret_cast<void **>(&dates), 32,
                       len * sizeof(int32_t));
	} else {
		this->dates = nullptr;
	}
	memoryUsage += (long) sizeof(int) * len;
}

void DateColumnVector::close() {
	if(!closed) {
		if(encoding && dates != nullptr) {
			free(dates);
		}
		dates = nullptr;
		ColumnVector::close();
	}
}

void DateColumnVector::print(int rowCount) {
	for(int i = 0; i < rowCount; i++) {
		std::cout<<dates[i]<<std::endl;
	}
}

DateColumnVector::~DateColumnVector() {
	if(!closed) {
		DateColumnVector::close();
	}
}

/**
     * Set a row from a value, which is the days from 1970-1-1 UTC.
     * We assume the entry has already been isRepeated adjusted.
     *
     * @param elementNum
     * @param days
 */
void DateColumnVector::set(int elementNum, int days) {
	if(elementNum >= writeIndex) {
		writeIndex = elementNum + 1;
	}
	dates[elementNum] = days;
	// TODO: isNull
}

void * DateColumnVector::current() {
    if(dates == nullptr) {
        return nullptr;
    } else {
        return dates + readIndex;
    }
}
void DateColumnVector::add(void* value) {
    int days = *(static_cast<int*>(value));  // 将void* 转换为int（日期的天数）
    ensureSize(writeIndex + 1);  // 确保空间足够
    dates[writeIndex++] = days;
}

void DateColumnVector::ensureSize(uint64_t size) {
    if (size > length) {
        // 扩展日期数组大小，可以使用类似2倍扩展策略，避免频繁扩展
        uint64_t newSize = std::max(size, length * 2);
        posix_memalign(reinterpret_cast<void**>(&dates), 32, newSize * sizeof(int32_t));
        length = newSize;
    }
}
