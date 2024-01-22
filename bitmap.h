/*
 * Header only library to help with working with bitmaps
 */

// The type used by the bitmap as storage
#define BITMAP_STORAGE unsigned int
// How many bits a single storage can hold
#define BITMAP_SLOT_SIZE (8 * sizeof(BITMAP_STORAGE))
// The mask needed to access element i in the correct storage
#define BITMAP_MASK(i) (1 << (i % BITMAP_SLOT_SIZE))
// How many storage elements are neeed for the element
#define BITMAP_STORAGE_NEEDED(elems)                                           \
  (elems / BITMAP_SLOT_SIZE + (elems % BITMAP_SLOT_SIZE > 0))
// Get the current value of v on the bitmap
#define BITMAP_GET(bitmap, v) (bitmap[v / BITMAP_SLOT_SIZE] & BITMAP_MASK(v))
// Sets the bit v on the bitmap
#define BITMAP_SET(bitmap, v) bitmap[v / BITMAP_SLOT_SIZE] |= BITMAP_MASK(v)
