int write2(int fd, const void * buf, size_t nbytes);

#define CASE(X, Y) \
{ \
case X : \
write2(1, #Y, 1); \
break; \
}


void printAddr(void * _addr)
{
    write2(1, "0x", 2);

    uint64_t addr = (uint64_t)_addr;

    for(int i = 17; i > 1; --i) {
        int addr_end = addr % 16;
        switch(addr_end) {
            CASE(0,  0);
            CASE(1,  1);
            CASE(2,  2);
            CASE(3,  3);
            CASE(4,  4);
            CASE(5,  5);
            CASE(6,  6);
            CASE(7,  7);
            CASE(8,  8);
            CASE(9,  9);
            CASE(10, A);
            CASE(11, B);
            CASE(12, C);
            CASE(13, D);
            CASE(14, E);
            CASE(15, F);
        }
        addr >>= 4;
    }
    write2(1, "\n", 1);
}
