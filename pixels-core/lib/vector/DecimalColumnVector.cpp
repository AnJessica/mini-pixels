//
// Created by yuly on 05.04.23.
//

#include "vector/DecimalColumnVector.h"

/**
 * The decimal column vector with precision and scale.
 * The values of this column vector are the unscaled integer value
 * of the decimal. For example, the unscaled value of 3.14, which is
 * of the type decimal(3,2), is 314. While the precision and scale
 * of this decimal are 3 and 2, respectively.
 *
 * <p><b>Note: it only supports short decimals with max precision
 * and scale 18.</b></p>
 *
 * Created at: 05/03/2022
 * Author: hank
 */

DecimalColumnVector::DecimalColumnVector(int precision, int scale, bool encoding): ColumnVector(VectorizedRowBatch::DEFAULT_SIZE, encoding) {
    DecimalColumnVector(VectorizedRowBatch::DEFAULT_SIZE, precision, scale, encoding);
}

DecimalColumnVector::DecimalColumnVector(uint64_t len, int precision, int scale, bool encoding): ColumnVector(len, encoding) {
	// decimal column vector has no encoding so we don't allocate memory to this->vector
	this->vector = nullptr;
    this->precision = precision;
    this->scale = scale;
    memoryUsage += (uint64_t) sizeof(uint64_t) * len;
}

void DecimalColumnVector::close() {
    if(!closed) {
        ColumnVector::close();
		vector = nullptr;
    }
}

void DecimalColumnVector::print(int rowCount) {
//    throw InvalidArgumentException("not support print Decimalcolumnvector.");
    for(int i = 0; i < rowCount; i++) {
        std::cout<<vector[i]<<std::endl;
    }
}

DecimalColumnVector::~DecimalColumnVector() {
    if(!closed) {
        DecimalColumnVector::close();
    }
}

void * DecimalColumnVector::current() {
    if(vector == nullptr) {
        return nullptr;
    } else {
        return vector + readIndex;
    }
}

int DecimalColumnVector::getPrecision() {
	return precision;
}


int DecimalColumnVector::getScale() {
	return scale;

}

void DecimalColumnVector::add(void* value) {
    int64_t decimalValue = *(static_cast<int64_t*>(value));  // 将 void* 转换为整数（无符号整数）
    ensureSize(writeIndex + 1);  // 确保空间足够
    vector[writeIndex++] = decimalValue;  // 添加到 vector 中
}

void DecimalColumnVector::ensureSize(uint64_t size) {
    if (size > length) {
        // 扩展 decimal 数组大小，可以使用类似2倍扩展策略，避免频繁扩展
        uint64_t newSize = std::max(size, length * 2);
        posix_memalign(reinterpret_cast<void**>(&vector), 32, newSize * sizeof(uint64_t));  // 重新分配内存
        length = newSize;  // 更新长度
    }
}
