#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

// Assume we need 32-byte alignment for AVX instructions

size_t getSize(int order)
{
    return 4096 * pow(2,order);
}

void *aligned_malloc(size_t size, int align)
{
    // We require whatever user asked for PLUS space for a pointer
    // PLUS space to align pointer as per alignment requirement
    void *mem = malloc(size + sizeof(void*) + (align - 1));

    // Location that we will return to user
    // This has space *behind* it for a pointer and is aligned
    // as per requirement
    void *ptr = (void**)((uintptr_t) (mem + (align - 1) + sizeof(void*)) & ~(align - 1));

    // Sneakily store address returned by malloc *behind* user pointer
    // void** cast is cause void* pointer cannot be decremented, cause
    // compiler has no idea "how many" bytes to decrement by
    ((void **) ptr)[-1] = mem;

    // Return user pointer
    return ptr;
}

void aligned_free(void *ptr)
{
    // Sneak *behind* user pointer to find address returned by malloc
    // Use that address to free
    free(((void**) ptr)[-1]);
}
/**
 * Эти две функции вы должны использовать для аллокации
 * и освобождения памяти в этом задании. Считайте, что
 * внутри они используют buddy аллокатор с размером
 * страницы равным 4096 байтам.
 **/

/**
 * Аллоцирует участок размером 4096 * 2^order байт,
 * выровненный на границу 4096 * 2^order байт. order
 * должен быть в интервале [0; 10] (обе границы
 * включительно), т. е. вы не можете аллоцировать больше
 * 4Mb за раз.
 **/

void *alloc_slab(int order){
/**
 * Освобождает участок ранее аллоцированный с помощью
 * функции alloc_slab.
 **/
    size_t as = getSize(order);
    return aligned_malloc(as,as);
}
void free_slab(void *slab){
    aligned_free(slab);
}

/**
 * Эта структура представляет аллокатор, вы можете менять
 * ее как вам удобно. Приведенные в ней поля и комментарии
 * просто дают общую идею того, что вам может понадобится
 * сохранить в этой структуре.
 **/
#define MIN_OBJECTS_NUM 1
#define MAX_OBJECTS_NUM 64

struct slab_t{
    uint8_t* data;
    size_t free_counter;
    slab_t* next;
};
struct cache {
    slab_t* empty_slabs;/* список пустых SLAB-ов для поддержки cache_shrink */
    slab_t* partial_slabs;/* список частично занятых SLAB-ов */
    slab_t* full_slabs;/* список заполненых SLAB-ов */

    size_t object_size; /* размер аллоцируемого объекта */
    int slab_order; /* используемый размер SLAB-а */
    size_t slab_objects; /* количество объектов в одном SLAB-е */
};



void getSlabParam(size_t obj_size, int *slab_order, size_t *obj_number){
   size_t slab_size = 4096;
   int tmp_order = 0;
   while (slab_size/obj_size < MAX_OBJECTS_NUM && tmp_order < 10)
   {
       tmp_order++;
       slab_size *= 2;
   }
   *slab_order = tmp_order;
   size_t tmp_number = slab_size/obj_size;
   size_t meta = tmp_number*sizeof(void*) + sizeof(slab_t);
   while (meta + obj_size*tmp_number > slab_size){
        tmp_number--;
        meta = tmp_number*sizeof(void*) + sizeof(slab_t);
   }
   *obj_number = tmp_number;
}

/**
 * Функция инициализации будет вызвана перед тем, как
 * использовать это кеширующий аллокатор для аллокации.
 * Параметры:
 *  - cache - структура, которую вы должны инициализировать
 *  - object_size - размер объектов, которые должен
 *    аллоцировать этот кеширующий аллокатор
 **/

void cache_setup(struct cache *cache, size_t object_size)
{
    cache->empty_slabs = NULL;
    cache->partial_slabs = NULL;
    cache->full_slabs = NULL;
    cache->object_size = object_size;
    getSlabParam(object_size, &cache->slab_order, &cache->slab_objects);
}
/**
 * Функция освобождения будет вызвана когда работа с
 * аллокатором будет закончена. Она должна освободить
 * всю память занятую аллокатором. Проверяющая система
 * будет считать ошибкой, если не вся память будет
 * освбождена.
 **/
void cache_release(struct cache *cache)
{
    /* Реализуйте эту функцию. */
}


/**
 * Функция аллокации памяти из кеширующего аллокатора.
 * Должна возвращать указатель на участок памяти размера
 * как минимум object_size байт (см cache_setup).
 * Гарантируется, что cache указывает на корректный
 * инициализированный аллокатор.
 **/
void *cache_alloc(struct cache *cache)
{
    /* Реализуйте эту функцию. */
}


/**
 * Функция освобождения памяти назад в кеширующий аллокатор.
 * Гарантируется, что ptr - указатель ранее возвращенный из
 * cache_alloc.
 **/
void cache_free(struct cache *cache, void *ptr)
{
    /* Реализуйте эту функцию. */
}


/**
 * Функция должна освободить все SLAB, которые не содержат
 * занятых объектов. Если SLAB не использовался для аллокации
 * объектов (например, если вы выделяли с помощью alloc_slab
 * память для внутренних нужд вашего алгоритма), то освбождать
 * его не обязательно.
 **/
void cache_shrink(struct cache *cache)
{
    /* Реализуйте эту функцию. */
}





int main()
{
    int slab_;
    size_t obj_;
    getSlabParam(512, &slab_, &obj_);

    cout << "Aligned pointer: " << alloc_slab(0) << endl;
    cout << "For " << obj_ << " objects of 512 bytes " << slab_ << " level slab fits" << endl;
    return 0;
}
