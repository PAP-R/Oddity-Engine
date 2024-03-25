struct Index {
    uint offset;
    uint base;
    uint count;
    uint indices;
};

uint get_index(inout uint current, Index index) {
    uint result = index.offset + current % index.base;
    current /= index.base;
    return result;
}

void add_index(uint index, inout Index indices) {
    if (indices.base <= index || index < indices.offset) return;
    indices.indices *= indices.base + index - offset;
    indices.count++;
}