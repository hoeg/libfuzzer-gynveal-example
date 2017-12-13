#include <cstdio>
#include <vector>
#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

class NetSock {
  std::vector<uint8_t> data;
  size_t idx;

 public:
  bool ReadBlob(const uint8_t *input, size_t size ) {
    data.assign(input, input+size);
    idx = 0;
    return true;
  }

  int WriteAll(const void */*Buffer*/, int /*Size*/) {}
  int Read(void *Buffer, int Size) {
    if (data.size() == idx) {
      return 0;
    }
    size_t sz = data[idx];

    idx ++;
    if (sz > data.size() - idx) {
      sz = data.size() - idx;
    }

    if (sz > Size) {
      sz = Size;
    }

    memcpy(Buffer, &data[idx], sz);
    idx += sz;
    return (int)sz;
  }
};

class Context {
 public:
  bool is_initilized{};
};

void HandlerAB(Context &ctx, NetSock *s) {
  char buf[4];
  s->Read(buf, 4);

  if (buf[0] == 't')
    if (buf[1] == 'e')
      if (buf[2] == 's')
        if (buf[3] == 't')
          ctx.is_initilized = true;

}

void HandlerXY(Context &ctx, NetSock *s) {
  if (!ctx.is_initilized) {
    return;
  }

  char buf[8];
  char output[16];
  s->Read(buf, 8);
  sprintf(output, buf);

  s->WriteAll(output, 16);
}

void Handler(NetSock *s) {
  char buf[4];
  Context ctx;

  for (;;) {
    if (s->Read(buf, 2) == 0) {
      break;
    }

    if (buf[0] == 'Q') {
      break;
    }

    if (buf[0] == 'A' && buf[1] == 'B') {
      HandlerAB(ctx, s);  // Init.
      if (!ctx.is_initilized) {
        return;
      }
    }

    if (buf[0] == 'X' && buf[1] == 'Y') {
      HandlerXY(ctx, s);  // Do some work.
    }
  }
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  NetSock c;
  c.ReadBlob(Data, Size);
  Handler(&c);
  return 0;
}
/*
int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: serv_harness <file>\n");
    return 1;
  }


  NetSock c;
  if (!c.ReadBlobFromFile(argv[1])) {
    puts("ReadBlobFromFile()");
    return 1;
  }
  Handler(&c);
  return 0;
}
*/
