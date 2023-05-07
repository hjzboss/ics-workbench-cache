#include "common.h"
#include <inttypes.h>

extern FILE *fd1;
extern FILE *fd2;

typedef struct {
  bool valid;
  bool dirty;
  uint64_t tag;
  uint8_t *data;
} cache_entry, *cache_set;

static int entry_num;
static int tag_width, index_width;
static int set_num;
static int set_size;
static cache_set *cache; // 组相连cache

static uint64_t hit_num, miss_num;

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

// 抽取data[lo, hi]
static uint32_t get_bit(uint32_t data, int lo, int hi) {
  return (data >> lo) & ~(0xffffffff << (hi - lo + 1));
}

// cache替换算法
static cache_entry* random_replace(uint32_t set_no, uintptr_t addr) {
  assert(set_no >= 0 && set_no < set_num);
  int r = rand() % set_size; // 随机生成下标
  cache_entry e = cache[set_no][r];
  uint32_t tag = get_bit(addr, BLOCK_WIDTH + index_width, 31);
  uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t) * BLOCK_SIZE);
  assert(data);
  // 写回脏块
  if (e.valid && e.dirty) {
    mem_write(e.tag << index_width | set_no, e.data);
    free(e.data);
  }
  // 替换cache块
  mem_read(get_bit(addr, BLOCK_WIDTH, 31), data);
  cache[set_no][r] = (cache_entry){
    .tag = tag, .dirty = false, .valid = true, .data = data
  };
  return &(cache[set_no][r]);
}

// 从 cache 中读出 addr 地址处的 4 字节数据
// 若缺失，需要先从内存中读入数据
uint32_t cache_read(uintptr_t addr) {
  uint32_t index = get_bit(addr, BLOCK_WIDTH, BLOCK_WIDTH + index_width - 1);
  uint32_t tag = get_bit(addr, BLOCK_WIDTH + index_width, 31);
  uint32_t offset = get_bit(addr, 0, BLOCK_WIDTH - 1) & ~0x3; // 地址对齐

  bool hit = false;
  uint32_t data;
  for (int i = 0; i < set_size; i++) {
    cache_entry t = cache[index][i];
    if (t.tag == tag && t.valid == true) {
      hit = true;
      hit_num++;
      data = *(uint32_t *)(t.data + offset);
      break;
    }
  }
  if (!hit) {
    miss_num++;
    // cache替换算法
    cache_entry *t = random_replace(index, addr);
    assert(t);
    data = *(uint32_t *)(t->data + offset);
  }
  return data;
}

// 往 cache 中 addr 地址所属的块写入数据 data，(写掩码为 wmask
// 例如当 wmask 为 0xff 时，只写入低8比特
// 若缺失，需要从先内存中读入数据
void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  uint32_t index = get_bit(addr, BLOCK_WIDTH, BLOCK_WIDTH + index_width - 1);
  uint32_t tag = get_bit(addr, BLOCK_WIDTH + index_width, 31);
  uint32_t offset = get_bit(addr, 0, BLOCK_WIDTH - 1) & ~0x3; // 地址对齐

  bool hit = false;
  for (int i = 0; i < set_size; i++) {
    cache_entry *t = &(cache[index][i]);
    if (t->tag == tag && t->valid == true) {
      hit = true;
      hit_num++;
      t->dirty = true;
      uint32_t old_data = *(uint32_t *)(t->data + offset);
      *(uint32_t *)(t->data + offset) = (old_data & ~wmask) | (data & wmask);
      break;
    }
  }

  if (!hit) {
    miss_num++;
    cache_entry *t = random_replace(index, addr);
    assert(t);
    uint32_t old_data = *(uint32_t *)(t->data + offset);
    *(uint32_t *)(t->data + offset) = (old_data & ~wmask) | (data & wmask);
    t->dirty = true;
  }
}

// 初始化一个数据大小为 2^total_size_width B，关联度为 2^associativity_width 的 cache
// 例如 init_cache(14, 2) 将初始化一个 16KB，4 路组相联的cache
// 将所有 valid bit 置为无效即可
void init_cache(int total_size_width, int associativity_width) {
  miss_num = hit_num = cycle_cnt = 0;
  entry_num = exp2(total_size_width - BLOCK_WIDTH);
  set_size = exp2(associativity_width);
  set_num = entry_num / set_size;
  tag_width = MEM_WIDTH - total_size_width + associativity_width;
  index_width = total_size_width - BLOCK_WIDTH - associativity_width;

  cache = (cache_set *)malloc(set_num * sizeof(cache_set));

  for (int i = 0; i < set_num; i++) {
    cache[i] = (cache_set)malloc(set_size * sizeof(cache_entry));
    for (int j = 0; j < set_size; j++) {
      cache[i][j].valid = false;
      cache[i][j].dirty = false;
      cache[i][j].data = (uint8_t *)malloc(sizeof(uint8_t) * BLOCK_SIZE);
    }
  }
}


void display_statistic(void) {
  //printf("\nstatistic:\n");
  double hit_rate = hit_num / 1.0 / (miss_num + hit_num);
  printf("(%d %d %d) ", entry_num, set_num, set_size);
  printf("total=%ld, hit=%ld, miss=%ld, ", miss_num + hit_num, hit_num, miss_num);
  printf("hit_rate=%f, cycle=%ld\n", hit_rate, cycle_cnt);
  fprintf(fd1, "%lf\n", hit_rate);
  //fprintf(fd2, "%ld\n", cycle_cnt);
}
