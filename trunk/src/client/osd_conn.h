
struct write_buf* write_buf_hash_find(uint32_t ino) ;
void buffered_write(uint32_t ino, uint64_t offset, uint32_t size, const uint8_t * buff);
void buffered_write_flush(uint32_t ino);


