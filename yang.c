struct kfifo{
	uint8_t *buffer;
	uint32_t in; // 输入指针
	uint32_t out; // 输出指针
	uint32_t size; // 缓冲区大小，必须为2的次幂
}
kfifo(uint32_t _size)
{
	if (!is_power_of_2(_size))
		_size = roundup_power_of_2(_size);

	buffer = new uint8_t[_size];
	in = 0;
	out = 0;
	size = _size;
}

// 返回实际写入缓冲区中的数据
uint32_t put(const uint8_t *data, uint32_t len)
{
	// 当前缓冲区空闲空间
	len = min(len,size - in + out);

	// 当前in位置到buffer末尾的长度
	auto l = min(len, size - (in  & (size - 1)));

	// 首先复制数据到[in，buffer的末尾]
	memcpy(buffer + (in & (size - 1)), data, l);

	// 复制剩余的数据（如果有）到[buffer的起始位置,...]
	memcpy(buffer, data + l, len - l);

	in += len; // 直接加，不作模运算。当溢出时，从buffer的开始位置重新开始

	return len;
}

// 返回实际读取的数据长度
uint32_t get(uint8_t *data, uint32_t len)
{
	// 缓冲区中的数据长度
	len = min(len, in - out);

	// 首先从[out,buffer end]读取数据
	auto l = min(len, size - (out & (size - 1)));
	memcpy(data, buffer + (out & (size - 1)), l);

	// 从[buffer start,...]读取数据
	memcpy(data + l, buffer, len - l);

	out += len; // 直接加，不错模运算。溢出后，从buffer的起始位置重新开始

	return len;
}
