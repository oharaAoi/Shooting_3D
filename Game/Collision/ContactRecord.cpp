#include "ContactRecord.h"

ContactRecord::ContactRecord() {
}

ContactRecord::~ContactRecord() {
}

void ContactRecord::AddRecord(const uint32_t& number) {
	record_.push_back(number);
}

bool ContactRecord::CheckRecord(const uint32_t& number) {
	return std::any_of(record_.begin(), record_.end(), [number](uint32_t recordNumber) {
		return recordNumber == number;
	});
}

void ContactRecord::Clear() {
	record_.clear();
}
