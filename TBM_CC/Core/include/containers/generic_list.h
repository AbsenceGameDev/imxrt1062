// /**
//  * @authors   Ario Amin @ Permadev, 
//  * @copyright Copyright (c) 2021-2026, MIT-License included in project toplevel dir
//  */

// #ifndef GENERIC_LIST_H
// #define GENERIC_LIST_H

// #include <stdint.h>

// typedef void (* __list_push)    (void* list, ... /* void* data */);
// typedef void (* __list_resize)  (void* list, int new_size);
// typedef void (* __newbuffer)    (void* list, int element_count);
// typedef void (* __list_destroy) (void* list);

// void list_new_buffer_(void* existing_list, int element_count);
// void list_resize(void* list, int new_size);
// void list_delete(void* list);
// void list_delete_buffer(void* list);
// void list_push(void* list, ... /* void* data */);

// struct __BaseList
// {
//     void* buffer;
//     int   index;
//     int   length;
//     int   capacity;
//     int   single_element_size;
//     __list_push    push;
//     __list_resize  resize;
//     __newbuffer    newbuffer;
//     __list_destroy destroy;
// } _BaseList =
//     { 
//         .buffer              = ((void*)0), 
//         .index               = 0, 
//         .length              = 0, 
//         .capacity            = 0, 
//         .single_element_size = 0,
//         .push                = list_push, 
//         .resize              = list_resize, 
//         .newbuffer           = list_new_buffer_, 
//         .destroy             = list_delete_buffer
//     };

// typedef struct __BaseList BaseList;


// #define List(type)                \
//     struct                        \
//     {                             \
//         type *buffer;             \
//         int   element_size;       \
//         int   index;              \
//         int   length;             \
//         int   capacity;           \
//         int   single_element_size;\
//         __list_push    push;      \
//         __list_resize  resize;    \
//         __newbuffer    newbuffer; \
//         __list_destroy destroy;   \
//     } *



// BaseList* list_new(int size_of_each_element, int element_count);
// // #define list_new(type, elementcount) (List(type) ) list_new_(sizeof(type), elementcount);



// #endif // GENERIC_LIST_H